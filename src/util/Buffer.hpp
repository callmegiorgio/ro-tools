#ifndef RO_BUFFER_HPP
#define RO_BUFFER_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <stdexcept>

class Buffer {
public:
    /// Constructs a new buffer with a given size.
    explicit Buffer(size_t size = 0) { data_.resize(size); }

    /// Constructs from a memory pointer and becomes its owner.
    explicit Buffer(std::vector<uint8_t> data) noexcept
        : data_{ std::move(data) } {}

    /// Pointer to the data.
    uint8_t* data() noexcept { return data_.data(); }

    /// Const pointer to the data.
    const uint8_t* data() const noexcept { return data_.data(); }

    /// Size of the buffer.
    size_t size() const noexcept { return data_.size(); }

    /// Number of bytes remaining before the end of the buffer.
    size_t remaining() const noexcept { return size() - current_idx_; }

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

    /// Writes n bytes from src into the buffer, allocating space as needed.
    void write(const void* src, size_t n)
    {
        try {
            // Check whether there's available space for writing in the next n positions.
            checkRange(n);
        }
        catch (const std::out_of_range&) {
            grow(n);
        }

        std::memcpy(&data_[current_idx_], src, n);
        current_idx_ += n;
    }

    /**
     * Increases the buffer's size in n bytes.
     *
     * @throws std::bad_alloc if allocation fails.
     */
    void grow(size_t n) { data_.resize(size() + n); }

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

    /// Reads numbers with range checking.
    uint8_t  readUint8()  const { return readNumber<uint8_t>(); }
    uint16_t readUint16() const { return readNumber<uint16_t>(); }
    uint32_t readUint32() const { return readNumber<uint32_t>(); }
    uint64_t readUint64() const { return readNumber<uint64_t>(); }
    int8_t   readInt8()   const { return static_cast<int8_t>(readUint8()); }
    int16_t  readInt16()  const { return static_cast<int16_t>(readUint16()); }
    int32_t  readInt32()  const { return static_cast<int32_t>(readUint32()); }
    int64_t  readInt64()  const { return static_cast<int64_t>(readUint64()); }
    float    readFloat()  const { return readNumber<float>(); }
    double   readDouble() const { return readNumber<double>(); }

    /// Reads numbers without range checking.
    uint8_t  getUint8()  const noexcept { return getNumber<uint8_t>(); }
    uint16_t getUint16() const noexcept { return getNumber<uint16_t>(); }
    uint32_t getUint32() const noexcept { return getNumber<uint32_t>(); }
    uint64_t getUint64() const noexcept { return getNumber<uint64_t>(); }
    int8_t   getInt8()   const noexcept { return static_cast<int8_t>(getUint8()); }
    int16_t  getInt16()  const noexcept { return static_cast<int16_t>(getUint16()); }
    int32_t  getInt32()  const noexcept { return static_cast<int32_t>(getUint32()); }
    int64_t  getInt64()  const noexcept { return static_cast<int64_t>(getUint64()); }
    float    getFloat()  const noexcept { return getNumber<float>(); }
    double   getDouble() const noexcept { return getNumber<double>(); }

    /// Writes numbers allocating memory as needed.
    void writeUint8(uint8_t value)   { writeNumber<uint8_t>(value); }
    void writeUint16(uint16_t value) { writeNumber<uint16_t>(value); }
    void writeUint32(uint32_t value) { writeNumber<uint32_t>(value); }
    void writeUint64(uint64_t value) { writeNumber<uint64_t>(value); }
    void writeInt8(int8_t value)     { writeUint8(static_cast<int8_t>(value)); }
    void writeInt16(int16_t value)   { writeUint16(static_cast<int16_t>(value)); }
    void writeInt32(int32_t value)   { writeUint32(static_cast<int32_t>(value)); }
    void writeInt64(int64_t value)   { writeUint64(static_cast<int64_t>(value)); }
    void writeFloat(float value)     { writeNumber<float>(value); }
    void writeDouble(double value)   { writeNumber<double>(value); }

    /// Writes numbers without memory allocation.
    void setUint8(uint8_t value)   noexcept { setNumber<uint8_t>(value); }
    void setUint16(uint16_t value) noexcept { setNumber<uint16_t>(value); }
    void setUint32(uint32_t value) noexcept { setNumber<uint32_t>(value); }
    void setUint64(uint64_t value) noexcept { setNumber<uint64_t>(value); }
    void setInt8(int8_t value)     noexcept { setUint8(static_cast<int8_t>(value)); }
    void setInt16(int16_t value)   noexcept { setUint16(static_cast<int16_t>(value)); }
    void setInt32(int32_t value)   noexcept { setUint32(static_cast<int32_t>(value)); }
    void setInt64(int64_t value)   noexcept { setUint64(static_cast<int64_t>(value)); }
    void setFloat(float value)     noexcept { setNumber<float>(value); }
    void setDouble(double value)   noexcept { setNumber<double>(value); }

private:
    /// Generic throwing numeric read function.
    template <typename Number>
    Number readNumber() const
    {
        checkRange(sizeof(Number));
        return getNumber<Number>();
    }

    /// Generic non-throwing numeric read function.
    template <typename Number>
    Number getNumber() const noexcept
    {
        Number value = *reinterpret_cast<const Number*>(&data_[current_idx_]);
        current_idx_ += sizeof(Number);
        return value;
    }

    /// Generic buffer-growing numeric write function.
    template <typename Number>
    void writeNumber(Number value)
    {
        try {
            checkRange(sizeof(Number));
        }
        catch (const std::out_of_range&) {
            grow(sizeof(Number));
        }

        setNumber<Number>(value);
    }

    /// Generic non-throwing number write function.
    template <typename Number>
    void setNumber(Number value) noexcept
    {
        *reinterpret_cast<Number*>(&data_[current_idx_]) = value;
        current_idx_ += sizeof(Number);
    }

    /// Throws an std::out_of_range exception if trying to access an index beyond buffer range.
    void checkRange(size_t n) const { data_.at(current_idx_ + n - 1); }

    std::vector<uint8_t> data_;
    mutable size_t current_idx_ = 0;
};

static_assert(sizeof(float) == 4, "float size is not 4 bytes");
static_assert(sizeof(double) == 8, "double size is not 8 bytes");

#endif // RO_BUFFER_HPP