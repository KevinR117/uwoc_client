#include <cstdint>
#include <mutex>
#include <iostream>

#pragma once

/** \name               Buffer
 *  \brief              Implementation of a circular buffer
**/
class Buffer
{
public:

    /*******************************************************
     * \section             Constructor, destructor
     *
    *******************************************************/

    /** \brief              Assign constructor
     *  \param[in]          size_ Capacity of the circular buffer
    **/
    explicit Buffer(uint32_t size_);

    /** \brief              Destructor
    **/
    ~Buffer();

    /*******************************************************
     * \section             Methods, functions
     *
    *******************************************************/

    /** \brief              Initialize buffer object
    **/
    void init();

    /** \brief              Get the capacity of the buffer
    **/
    uint32_t size() const;

    /** \brief              Access to the buffer content
     *  \return             Array containing data
    **/
    uint8_t** buff() const;

    /** \brief              Get the begin index of the used space in the buffer
     *  \return             Index
    **/
    uint32_t beginIndex() const;

    /** \brief              Get the end index (excluded) of the used space in the buffer
     *  \return             Index
    **/
    uint32_t endIndex() const;

    /** \brief              Tells if the buffer contains over <threshold> values that are still not read
     *  \param[in]          threshold_ Will return true only if there are over <threshold> values that are not read
     *  \return             True if enough data is available for reading
    **/
    bool available(uint32_t threshold_) const;

    /** \brief              Add values into the free space of the buffer
     *  \param[in]          data_ Data to add
     *  \param[in]          size_ Length of the data array
    **/
    void addVals(const uint8_t* data_, uint32_t size_);

    /** \brief              Read available data (which is still not read)
     *  \param[in]          allocatedSize_ Allocated memory for reading from the buffer
     *  \param[out]         dataOut_ Buffer that will contains the read values from the buffer
     *  \return             Result length (not necessarily equal to the allocated size)
    **/
    uint32_t readAvailable(uint32_t allocatedSize_, uint8_t* dataOut_);

private:

    /*******************************************************
     * \section             Private members
     *
    *******************************************************/
    
    /// \brief              Buffer capacity
    uint32_t m_buffSize;

    /// \brief              Content of the buffer
    uint8_t** m_content = nullptr;

    /// \brief              Begin index of the still not read data (available)
    uint32_t m_beginRead = 0;

    /// \brief              End index of the still not read data (available)
    uint32_t m_endRead = 0; // Excluded

    /// \brief              Mutex protection for thread safe applications
    std::mutex m_mutex;
};
