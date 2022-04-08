#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <atomic>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#pragma once

using boost::asio::ip::tcp;
using namespace boost::asio;

/** \name               Client
 *  \brief              The client will be in charge of sending video to the server (running on the RP card) and show
 *  the received video from it
**/
class Client
{
public:

    /*******************************************************
     * \section             Constructor, destructor
     *
    *******************************************************/
    
    /** \brief              Assign constructor
     *  \param[in]          port_ Server port on which we want to connect
     *  \param[in]          cols_ Number of columns in the video frames
     *  \param[in]          rows_ Number of rows in the video frames
     *  \return             
    **/
    explicit Client(uint16_t port_, uint32_t cols_, uint32_t rows_);

    /** \brief              Destructor
    **/
    ~Client();
    
    /*******************************************************
     * \section             Methods and functions
     *
    *******************************************************/

    /** \brief              Initialise the client (connect to the server)
    **/
    void init();

    /** \brief              Uninitialize the client (disconnect from the server)
    **/
    void uninit();

    /** \brief              Get the last received frame from the server
     *  \return             Pointer over the frame
    **/
    cv::Mat* img();

    /** \brief              Send a packet to the server
     *  \param[in]          buff_ Packet to send
     *  \param[in]          size_ Size of the packet
    **/
    void send(uint8_t* buff_, size_t size_);

private:

    /*******************************************************
     * \section             Private methods and functions
     *
    *******************************************************/
    
    /** \brief              Start connection with the distant server
    **/
    void connectToHost();

    /** \brief              Wait for received messages from the server
    **/
    void recv();

    /*******************************************************
     * \section             Constants
     *
    *******************************************************/
    
    /// \brief              Buffer size for receiving and queuing frames before being processed
    static constexpr const uint32_t DATA_SIZE = 32768;

    /// \brief              Default IP of the distant server (needs to be configured according to the server conf)
    static constexpr const char* DEFAULT_IP = "192.168.1.20";

    /*******************************************************
     * \section             Private members
     *
    *******************************************************/
    
    /// \brief              Thread for handling asynchronous operations 
    std::thread m_asyncThread;

    /// \brief              Thread for receiving data from the server
    std::thread m_receivingThread;

    /// \brief              Thread safe bool value for knowing when to stop receive thread
    std::atomic<bool>* m_stopRecvThread;

    /// \brief              Context for the connection of the socket to server
    boost::asio::io_context m_ioContext;
    
    /// \brief              Socket in charge of receiving and sending data from/to the server
    tcp::socket m_socket;

    /// \brief              Object representing the server address (IP and port)
    tcp::endpoint m_endpoint;

    /// \brief              Buffer for queuing received messages from the server
    uint8_t* m_data;

    /// \brief              Buffer that contains the last uncompressed received image
    cv::Mat* m_currentImage;

    /// \brief              Temporary buffer that contains the last compressed received image
    std::vector<uchar> m_tempBuffer;
};