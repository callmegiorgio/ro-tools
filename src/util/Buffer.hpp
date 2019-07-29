#ifndef RO_BUFFER_HPP
#define RO_BUFFER_HPP

#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>

class Buffer {
public:
    // Constructs from a memory pointer and becomes its owner.
    explicit Buffer(uint8_t* data, size_t size) noexcept
        : data_{ data }
        , size_{ size } {}

    // Pointer to the data.
    uint8_t* data() noexcept { return data_.get(); }

    // Const pointer to the data.
    const uint8_t* data() const noexcept { return data_.get(); }

    // Size of the buffer.
    size_t size() const noexcept { return size_; }

    // Number of bytes remaining to be read.
    size_t remaining() const noexcept { return size_ - read_pos_; }

    /**
     * Reads n bytes into dest.
     *
     * @throws std::out_of_range if n exceeds buffer range.
     */
    void read(void* dest, size_t n)
    {
        checkRange(n);
        std::memcpy(dest, &data_[read_pos_], n);
        read_pos_ += n;
    }

    /**
     * Skips n bytes of data.
     *
     * @throws std::out_of_range if n exceeds buffer range.
     */
    void skip(size_t n)
    {
        checkRange(n);
        read_pos_ += n;
    }

    // Reads unsigned integers with range checking
    uint8_t  readUint8() { return readIntegral<uint8_t>(); }
    uint16_t readUint16() { return readIntegral<uint16_t>(); }
    uint32_t readUint32() { return readIntegral<uint32_t>(); }
    uint64_t readUint64() { return readIntegral<uint64_t>(); }

    // Reads signed integers with range checking
    int8_t  readInt8() { return static_cast<int8_t>(readUint8()); }
    int16_t readInt16() { return static_cast<int16_t>(readUint16()); }
    int32_t readInt32() { return static_cast<int32_t>(readUint32()); }
    int64_t readInt64() { return static_cast<int64_t>(readUint64()); }

    // Reads unsigned integers without range checking
    uint8_t  getUint8() noexcept { return getIntegral<uint8_t>(); }
    uint16_t getUint16() noexcept { return getIntegral<uint16_t>(); }
    uint32_t getUint32() noexcept { return getIntegral<uint32_t>(); }
    uint64_t getUint64() noexcept { return getIntegral<uint64_t>(); }

    // Reads signed integers without range checking
    int8_t  getInt8() noexcept { return static_cast<int8_t>(getUint8()); }
    int16_t getInt16() noexcept { return static_cast<int16_t>(getUint16()); }
    int32_t getInt32() noexcept { return static_cast<int32_t>(getUint32()); }
    int64_t getInt64() noexcept { return static_cast<int64_t>(getUint64()); }

    // Extraction operators
    //Buffer& operator>>(uint8_t& value) { value = readUint8(); return *this; }
    //Buffer& operator>>(uint16_t& value) { value = readUint16(); return *this; }
    //Buffer& operator>>(uint32_t& value) { value = readUint32(); return *this; }
    //Buffer& operator>>(uint64_t& value) { value = readUint64(); return *this; }

private:
    template <typename Integral>
    Integral readIntegral()
    {
        checkRange(sizeof(Integral));
        return getIntegral<Integral>();
    }

    template <typename Integral>
    Integral getIntegral()
    {
        Integral value = *reinterpret_cast<Integral*>(&data_[read_pos_]);
        read_pos_ += sizeof(Integral);
        return value;
    }

    void checkRange(size_t index) const
    {
        if ((read_pos_ + index) > size_)
            throw std::out_of_range("Buffer::checkRange");
    }

    std::unique_ptr<uint8_t[]> data_;
    size_t size_;
    size_t read_pos_ = 0;
};

#endif // RO_BUFFER_HPP