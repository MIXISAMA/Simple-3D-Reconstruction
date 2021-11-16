#pragma once

#include "pch.h"
#include "util/image.h"

namespace mixi
{

class Calibration
{
public:

    enum class Pattern
    {
        CHESSBOARD,
        CIRCLES_GRID,
        ASYMMETRIC_CIRCLES_GRID,
    };

    Calibration(
        int boardWidth = 8,
        int boardHeight = 5,
        float squareSize = 18.0f,
        Pattern pattern = Pattern::CHESSBOARD
    );
    ~Calibration();

    // Number of inner corners per a item row and column. (square, circle)
    int boardWidth;
    int boardHeight;

    // The size of a square in some user defined metric system (pixel, millimeter)
    float squareSize;

    Pattern pattern;

    bool findCornersFast(void* data, int bytes, int width, int height, std::vector<cv::Point2f>& corners);

    void drawCornersFast(void* data, int bytes, int width, int height, std::vector<cv::Point2f>& corners);

    double calibrate(std::vector<Image::Ptr>& images, cv::Mat& intrinsic, cv::Mat& distCoeffs);

    bool findCorners(const Image* image, std::vector<cv::Point2f>& corners);

    void undistort(
        void* data, int bytes, int width, int height,
        void* outData,
        const cv::Mat& intrinsic,
        const cv::Mat& distCoeffs
    );

private:

    void calcBoardCornerPositions(std::vector<cv::Point3f>& corners);

    
};

} // namespace mixi