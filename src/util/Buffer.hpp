#ifndef RO_BUFFER_HPP
#define RO_BUFFER_HPP

#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>

class Buffer {
public:
    /// Constructs a new buffer with a given size.
    explicit Buffer(size_t size = 0) { resize(size); }

    /// Constructs from a memory pointer and becomes its owner.
    explicit Buffer(uint8_t* data, size_t size) noexcept
        : data_{ data }
        , size_{ size } {}

    /// Pointer to the data.
    uint8_t* data() noexcept { return data_.get(); }

    /// Const pointer to the data.
    const uint8_t* data() const noexcept { return data_.get(); }

    /// Size of the buffer.
    size_t size() const noexcept { return size_; }

    /// Number of bytes remaining before the end of the buffer.
    size_t remaining() const noexcept { return size_ - current_idx_; }

    /**
     * Reads n bytes into dest.
     *
     * @throws std::out_of_range if n exceeds buffer range.
     */
    void read(void* dest, size_t n) const
    {
        checkRange(n);
        std::memcpy(dest, &data_[current_idx_], n);
        current_idx_ += n;
    }

    /// Writes n bytes from src into the buffer, reallocating space as needed.
    void write(const void* src, size_t n)
    {
        try {
            // Check whether there's available space for writing in the next n positions.
            checkRange(n);
        }
        catch (const std::out_of_range&) {
            resize(size() + n);
        }

        std::memcpy(&data_[current_idx_], src, n);
        current_idx_ += n;
    }

    /// Resize the buffer's memory size. If the buffer is shrunk, the cursor is moved to the last position.
    void resize(size_t new_size)
    {
        if (new_size == size_)
            return;

        uint8_t* new_data = nullptr;

        if (new_size) {
            new_data = new uint8_t[new_size];

            if (size_)
                std::memcpy(new_data, data_.get(), std::min(new_size, size_));
        }

        if (new_size < size_)
            current_idx_ = new_size - 1;

        data_.reset(new_data);
        size_ = new_size;
    }

    /**
     * Skips n bytes of data.
     *
     * @throws std::out_of_range if n exceeds buffer range.
     */
    void skip(size_t n) const
    {
        checkRange(n);
        current_idx_ += n;
    }

    /// Reads integers with range checking.
    uint8_t  readUint8()  const { return readIntegral<uint8_t>(); }
    uint16_t readUint16() const { return readIntegral<uint16_t>(); }
    uint32_t readUint32() const { return readIntegral<uint32_t>(); }
    uint64_t readUint64() const { return readIntegral<uint64_t>(); }
    int8_t   readInt8()   const { return static_cast<int8_t>(readUint8()); }
    int16_t  readInt16()  const { return static_cast<int16_t>(readUint16()); }
    int32_t  readInt32()  const { return static_cast<int32_t>(readUint32()); }
    int64_t  readInt64()  const { return static_cast<int64_t>(readUint64()); }

    /// Reads integers without range checking.
    uint8_t  getUint8()  const noexcept { return getIntegral<uint8_t>(); }
    uint16_t getUint16() const noexcept { return getIntegral<uint16_t>(); }
    uint32_t getUint32() const noexcept { return getIntegral<uint32_t>(); }
    uint64_t getUint64() const noexcept { return getIntegral<uint64_t>(); }
    int8_t   getInt8()   const noexcept { return static_cast<int8_t>(getUint8()); }
    int16_t  getInt16()  const noexcept { return static_cast<int16_t>(getUint16()); }
    int32_t  getInt32()  const noexcept { return static_cast<int32_t>(getUint32()); }
    int64_t  getInt64()  const noexcept { return static_cast<int64_t>(getUint64()); }

    /// Writes integers with range checking.
    void writeUint8(uint8_t value)   { writeIntegral<uint8_t>(value); }
    void writeUint16(uint16_t value) { writeIntegral<uint16_t>(value); }
    void writeUint32(uint32_t value) { writeIntegral<uint32_t>(value); }
    void writeUint64(uint64_t value) { writeIntegral<uint64_t>(value); }
    void writeInt8(int8_t value)     { writeUint8(static_cast<int8_t>(value)); }
    void writeInt16(int16_t value)   { writeUint16(static_cast<int16_t>(value)); }
    void writeInt32(int32_t value)   { writeUint32(static_cast<int32_t>(value)); }
    void writeInt64(int64_t value)   { writeUint64(static_cast<int64_t>(value)); }

    /// Writes integers without range checking.
    void setUint8(uint8_t value)   noexcept { setIntegral<uint8_t>(value); }
    void setUint16(uint16_t value) noexcept { setIntegral<uint16_t>(value); }
    void setUint32(uint32_t value) noexcept { setIntegral<uint32_t>(value); }
    void setUint64(uint64_t value) noexcept { setIntegral<uint64_t>(value); }
    void setInt8(int8_t value)     noexcept { setUint8(static_cast<int8_t>(value)); }
    void setInt16(int16_t value)   noexcept { setUint16(static_cast<int16_t>(value)); }
    void setInt32(int32_t value)   noexcept { setUint32(static_cast<int32_t>(value)); }
    void setInt64(int64_t value)   noexcept { setUint64(static_cast<int64_t>(value)); }

private:
    /// Generic throwing integral read function.
    template <typename Integral>
    Integral readIntegral() const
    {
        checkRange(sizeof(Integral));
        return getIntegral<Integral>();
    }

    /// Generic non-throwing integral read function.
    template <typename Integral>
    Integral getIntegral() const noexcept
    {
        Integral value = *reinterpret_cast<Integral*>(&data_[current_idx_]);
        current_idx_ += sizeof(Integral);
        return value;
    }

    /// Generic throwing integral write function.
    template <typename Integral>
    void writeIntegral(Integral value)
    {
        checkRange(sizeof(Integral));
        setIntegral<Integral>(value);
    }

    /// Generic non-throwing integral write function.
    template <typename Integral>
    void setIntegral(Integral value) noexcept
    {
        *reinterpret_cast<Integral*>(&data_[current_idx_]) = value;
        current_idx_ += sizeof(Integral);
    }

    /// Throws an std::out_of_range exception if trying to access an index beyond buffer range.
    void checkRange(size_t index) const
    {
        if ((current_idx_ + index) > size_)
            throw std::out_of_range("Buffer::checkRange");
    }

    std::unique_ptr<uint8_t[]> data_;
    size_t size_ = 0;
    mutable size_t current_idx_ = 0;
};

#endif // RO_BUFFER_HPP