#include "process/calibration.h"

namespace mixi
{

Calibration::Calibration(
    int boardWidth,
    int boardHeight,
    int squareSize,
    Pattern pattern) :
    boardWidth(boardWidth),
    boardHeight(boardHeight),
    squareSize(squareSize),
    pattern(pattern)
{

}

Calibration::~Calibration()
{

}

bool Calibration::findCornersFast(
    void* data,
    int bytes,
    int width,
    int height,
    std::vector<cv::Point2f>& corners)
{
    cv::Mat image = cv::Mat(height, width, CV_8UC3, (unsigned*)data);
    cv::Size patternSize(boardWidth, boardHeight);

    switch (pattern)
    {
    case Pattern::CHESSBOARD:
        return cv::findChessboardCorners(
            image, patternSize, corners,
            cv::CALIB_CB_ADAPTIVE_THRESH |
            cv::CALIB_CB_NORMALIZE_IMAGE |
            cv::CALIB_CB_FAST_CHECK
        );
    default:
        return false;
    }
}


void Calibration::drawCornersFast(
    void* data,
    int bytes,
    int width,
    int height,
    std::vector<cv::Point2f>& corners)
{
    cv::Mat image = cv::Mat(height, width, CV_8UC3, (unsigned*)data);
    cv::Size patternSize(boardWidth, boardHeight);
    cv::drawChessboardCorners(image, patternSize, cv::Mat(corners), true);
}

double Calibration::calibrate(std::vector<Image::Ptr>& images, cv::Mat& intrinsic, cv::Mat& distCoeffs)
{
    std::vector<std::vector<cv::Point2f>> imagePoints;
    for (const Image::Ptr& image : images) {
        std::vector<cv::Point2f> corners;
        if (findCorners(image.get(), corners)) {
            imagePoints.push_back(corners);
        }
    }

    // will be output
    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<float> reprojErrs;
    double totalAvgErr = 0;
    std::vector<cv::Point3f> newObjPoints;

    cv::Size imageSize(images[0]->width(), images[0]->height());
    int iFixedPoint = -1;

    std::vector<std::vector<cv::Point3f> > objectPoints(1);
    calcBoardCornerPositions(objectPoints[0]);
    float grid_width = squareSize * (boardWidth - 1);
    objectPoints[0][boardWidth - 1].x = objectPoints[0][0].x + grid_width;
    newObjPoints = objectPoints[0];
    objectPoints.resize(imagePoints.size(), objectPoints[0]);

    return cv::calibrateCameraRO(
        objectPoints,
        imagePoints,
        imageSize,
        iFixedPoint,
        intrinsic,
        distCoeffs,
        rvecs,
        tvecs,
        newObjPoints
        // cv::CALIB_USE_INTRINSIC_GUESS |
        // cv::CALIB_FIX_ASPECT_RATIO |
        // cv::CALIB_FIX_PRINCIPAL_POINT |
        // cv::CALIB_FIX_K1 |
        // cv::CALIB_FIX_K2 |
        // cv::CALIB_FIX_K3 |
        // cv::CALIB_FIX_K4 |
        // cv::CALIB_FIX_K5 |
        // cv::CALIB_USE_LU
    );

}

bool Calibration::findCorners(const Image* image, std::vector<cv::Point2f>& corners)
{
    bool found = false;
    cv::Mat rbg = cv::Mat(
        image->height(),
        image->width(),
        CV_8UC3,
        (unsigned*)image->data()
    );
    cv::Size patternSize(boardWidth, boardHeight);

    switch (pattern)
    {
    case Pattern::CHESSBOARD:
        found = cv::findChessboardCorners(rbg, patternSize, corners);
        break;
    default:
        return false;
    }
    if (!found) {
        return false;
    }

    cv::Mat gray;
    cvtColor(rbg, gray, cv::COLOR_BGR2GRAY);
    cornerSubPix(
        gray,
        corners,
        cv::Size(11, 11),
        cv::Size(-1,-1),
        cv::TermCriteria(
            cv::TermCriteria::EPS |
            cv::TermCriteria::COUNT,
            30,
            0.0001
        )
    );
    return true;
}

void Calibration::undistort(
    void* data, int bytes, int width, int height,
    void* outData,
    const cv::Mat& intrinsic,
    const cv::Mat& distCoeffs)
{
    cv::Mat rbg(height, width, CV_8UC3, (unsigned*)data);
    cv::Mat out(height, width, CV_8UC3, (unsigned*)outData);
    cv::undistort(rbg, out, intrinsic, distCoeffs);
}


void Calibration::calcBoardCornerPositions(std::vector<cv::Point3f>& corners)
{
    corners.clear();

    switch(pattern)
    {
    case Pattern::CHESSBOARD:
    case Pattern::CIRCLES_GRID:
        for (int i = 0; i < boardHeight; ++i) {
            for (int j = 0; j < boardWidth; ++j) {
                corners.push_back(
                    cv::Point3f(j * squareSize, i * squareSize, 0)
                );
            }
        }
        break;
    case Pattern::ASYMMETRIC_CIRCLES_GRID:
        for (int i = 0; i < boardHeight; i++) {
            for (int j = 0; j < boardWidth; j++) {
                corners.push_back(
                    cv::Point3f((2 * j + i % 2) * squareSize, i * squareSize, 0)
                );
            }
        }
        break;
    }
}

} // namespace mixi
