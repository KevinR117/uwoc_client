#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "client.h"

/** \brief              Main entry point of a client
 *  \param[in]          argc Args count
 *  \param[out]         argv Arguments array
 *  \return             integer
**/
int main()
{
    // Creation of a client object with distant server port and video size
    Client cli(7077, 1280, 720);

    // Needed to set an environment variable (useful for running server on computer for video)
    setenv("DISPLAY", ":0", 1);

    try
    {
        // Connect to server (running on the RP card)
        cli.init();
    }
    catch(std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }

    // Frame object (one image of the video)
    cv::Mat frame;

    // Open video file
    cv::VideoCapture cap("../ex_video.avi");

    if (!cap.isOpened())
    {
        std::cout << "Can't open video file" << std::endl;
    }

    // Wait 1s before starting to send data to the server
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Configuration for JPEG compression
    std::vector<uchar> compressed;
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_JPEG_QUALITY;
    param[1] = 80;

    // While all frames are not sent to the server
    while (true)
    {
        // Get the next frame of the video
        cap >> frame;
        if (frame.empty())
        {
            break;
        }

        // Show the sent frame
        imshow("sendFrame", frame);

        // Compress frame before sending it
        cv::imencode(".jpg", frame, compressed, param);

        // Send the frame to the distant server
        cli.send(compressed.data(), compressed.size());

        // Wait 5ms before showing received image
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        // If an image has been received from the server, print it
        cv::Mat* recvFrame = cli.img();
        if (recvFrame != nullptr)
        {
            if (!recvFrame->empty())
            {
                imshow("recvFrame", *recvFrame);
            }
        }

        // Waiting phase (if not here the image window will close itself)
        cv::waitKey(36);
    }

    // Release video object
    cap.release();

    // Close connection to the server
    cli.uninit();

    return 0;
}