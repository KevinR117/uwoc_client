#include "client.h"

/// \brief              Assign constructor
Client::Client(uint16_t port_, uint32_t cols_, uint32_t rows_)
        : m_socket(m_ioContext), m_endpoint(tcp::endpoint(boost::asio::ip::address::from_string(DEFAULT_IP), port_))
{
    m_stopRecvThread = new std::atomic<bool>();
    *m_stopRecvThread = false;

    m_data = new uint8_t[DATA_SIZE];

    m_currentImage = new cv::Mat(static_cast<int>(rows_), static_cast<int>(cols_), CV_8UC3);
    m_tempBuffer = std::vector<uchar>(0);
}

/// \brief              Destructor
Client::~Client()
{
    if (m_stopRecvThread != nullptr)
    {
        delete m_stopRecvThread;
        m_stopRecvThread = nullptr;
    }

    if (m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    if (m_currentImage != nullptr)
    {
        delete m_currentImage;
        m_currentImage = nullptr;
    }
}

/// \brief              Connect to the server
void Client::init()
{
    // Open connection with server
    this->connectToHost();

    // If well-connected, start socket thread and receive thread
    if (m_socket.is_open())
    {
        // Start async thread (socket)
        m_asyncThread = std::thread([this]() { m_ioContext.run(); });

        // Start receiving thread
        m_receivingThread = std::thread([this]() { this->recv(); });
    }
}

void Client::uninit()
{
    // Signal receive thread to stop
    *m_stopRecvThread = true;

    // Wait for threads to stop properly
    if (m_receivingThread.joinable())
    {
        m_receivingThread.join();
    }
    if (!m_ioContext.stopped())
    {
        m_ioContext.stop();
    }
    if (m_asyncThread.joinable())
    {
        m_asyncThread.join();
    }
    if (m_socket.is_open())
    {
        m_socket.close();
    }
}

/// \brief              Connect to host
void Client::connectToHost()
{
    // When the socket is connected to server, the callback is called
    m_socket.async_connect(m_endpoint, [](const boost::system::error_code& err_) {
        if (!err_)
        {
            std::cout << "Connected successfully to the server" << std::endl;
        }
        else
        {
            throw std::runtime_error(err_.message());
        }
    });
}

/// \brief              Send data to the server
void Client::send(uint8_t* buff_, size_t size_)
{
    // When the data is sent or an error occurs, the callback is called
    boost::asio::async_write(m_socket, boost::asio::buffer(buff_, size_), boost::asio::transfer_all(), [buff_](const boost::system::error_code& err_, size_t writtenBytes_)
    {
        if (err_)
        {
            std::cout << err_.message() << std::endl;
            throw std::runtime_error("Problem while sending data");
        }
        else
        {
            std::cout << "written " << writtenBytes_ << " bytes" << std::endl;
        }
    });
}

/// \brief              Wait for some packets received from the server
void Client::recv()
{
    if (*m_stopRecvThread)
    {
        return;
    }
    else
    {
        // When a packet is received, the callback is called
        boost::asio::async_read(m_socket, boost::asio::buffer(m_data, DATA_SIZE), [this](const boost::system::error_code& err_, size_t readBytes_)
        {
            if (readBytes_ == DATA_SIZE)
            {
                for (size_t index = 0; index < DATA_SIZE; index++)
                {
                    // When we recognize the end of a JPEG packet, we can build an image
                    if ((m_data[index] == 0xd9) && (m_tempBuffer[m_tempBuffer.size() - 1] == 0xff))
                    {
                        m_tempBuffer.push_back(m_data[index]);
                        std::cout << "Image size : " << m_tempBuffer.size() << std::endl;

                        // Uncompress image
                        cv::imdecode(m_tempBuffer, cv::IMWRITE_JPEG_QUALITY, m_currentImage);

                        // Reset the temporary buffer to wait for a new image
                        m_tempBuffer.clear();
                    }
                    else
                    {
                        // In other case, continue to wait for the end of a JPEG packet
                        m_tempBuffer.push_back(m_data[index]);
                    }
                }
            }
            
            // Wait for new packets from the server
            this->recv();
        });
    }
}

/// \brief              Return a pointer over the last received and uncompressed image
cv::Mat* Client::img()
{
    return m_currentImage;
}

