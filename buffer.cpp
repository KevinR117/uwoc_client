#include "buffer.h"

/// \brief              Constructor
Buffer::Buffer(uint32_t size_)
        : m_buffSize(size_)
{
}

/// \brief              Destructor
Buffer::~Buffer()
{
    if (m_content != nullptr) {
        for (uint32_t index = 0; index < m_buffSize; index++) {

            if (m_content[index] != nullptr)
            {
                delete m_content[index];
                m_content[index] = nullptr;
            }
        }

        delete[] m_content;
        m_content = nullptr;
    }
}

/// \brief              Init buffer content
void Buffer::init()
{
    // Initialize values
    m_content = new uint8_t*[m_buffSize];
    for (uint32_t index = 0; index < m_buffSize; index++)
    {
        m_content[index] = new uint8_t;
    }
}

/// \brief              Get the buffer capacity
uint32_t Buffer::size() const
{
    return m_buffSize;
}

/// \brief              Tells if enough data is available for reading
bool Buffer::available(uint32_t threshold_) const
{
    if (m_endRead < m_beginRead)
    {
        return ((m_buffSize - m_beginRead + m_endRead) >= threshold_);
    }
    else
    {
        return ((m_endRead - m_beginRead) >= threshold_);
    }
}

/// \brief              Add values to the buffer (into the free space)
void Buffer::addVals(const uint8_t *data_, uint32_t size_)
{
    if (m_content == nullptr)
    {
        throw std::runtime_error("Buffer not initialized");
    }
    if (size_ > m_buffSize)
    {
        throw std::invalid_argument("Data size too long");
    }
    else
    {
        // Mutual access protection
        m_mutex.lock();

        uint32_t tempEnd = m_endRead;
        for (uint32_t index = 0; index < size_; index++)
        {
            if (index == (m_buffSize - tempEnd))
            {
                m_endRead = 0;
            }
            *m_content[m_endRead] = data_[index];
            m_endRead++;
        }

        // End of the critical section
        m_mutex.unlock();
    }
}

/// \brief              Read data that is available
uint32_t Buffer::readAvailable(uint32_t allocatedSize_, uint8_t* dataOut_)
{
    if (m_content == nullptr)
    {
        throw std::runtime_error("Buffer not initialized");
    }
    else
    {
        // Mutual access protection
        m_mutex.lock();

        uint32_t resSize;
        if (m_endRead < m_beginRead)
        {
            resSize = m_buffSize - m_beginRead + m_endRead;
        }
        else
        {
            resSize = m_endRead - m_beginRead;
        }

        if (resSize > allocatedSize_)
        {
            throw std::invalid_argument("Allocated space memory for data out is not enough");
        }
        else {
            uint32_t tempBegin = m_beginRead;
            for (uint32_t index = 0; index < resSize; index++) {
                if (index == (m_buffSize - tempBegin)) {
                    m_beginRead = 0;
                }
                dataOut_[index] = *m_content[m_beginRead];
                m_beginRead++;
            }
        }

        // End of the critical section
        m_mutex.unlock();

        // The result size might be different from the allocated size
        return resSize;
    }
}

/// \brief              Access to the buffer content
uint8_t** Buffer::buff() const
{
    return m_content;
}

/// \brief              Begin index of the available data for reading zone
uint32_t Buffer::beginIndex() const
{
    return m_beginRead;
}

/// \brief              End index of the available data for reading zone
uint32_t Buffer::endIndex() const
{
    return m_endRead;
}