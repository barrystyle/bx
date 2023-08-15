#ifndef LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_HPP
#define LIBBITCOIN_SYSTEM_DATA_DATA_SLICE_HPP

#include <array>
#include <initializer_list>
#include <string>
#include <vector>

/// Lifetime:
/// "An rvalue may be used to initialize an rvalue reference, in which case
/// the lifetime of the object identified by the rvalue is extended until
/// the scope of the reference ends.":
/// en.cppreference.com/w/cpp/language/value_category#rvalue
/// A prvalue has the "same properties" as an rvalue:
/// en.cppreference.com/w/cpp/language/value_category#prvalue
/// A temporary is a prvalue, and includes initializer_list, but a copy is not:
/// en.cppreference.com/w/cpp/language/implicit_conversion#Temporary_materialization
/// When the data_slice is accepted as a const& it is either passed by
/// reference or materialized as a prvalue. A function/method reference extends
/// lifetime, but a member reference does not.

/// Downsizable but otherwise const iterable wrapper for const buffer.
/// Identical to data_slab except pointer is const, and therefore accepts
/// construction from const sources (including literals and initializers).
/// Not a substitute for move overrides or containment.
/// Accepts any sizeof(T) == 1 type as a "byte" and emits uint8_t.
/// [] iteration past end is safe and returns zeros.
/// Negative size construction yields a valid empty object.
class data_slice
{
public:
    typedef size_t size_type;
    typedef uint8_t value_type;

    /// A pointer to non-mutable bytes (pointer itself is mutable).
    typedef const value_type* pointer;

    /// For stream source compatibility, until data_slice has an iterator.
    typedef pointer const_iterator;

    /// Constructors.
    /// -----------------------------------------------------------------------

    /// Empty slice.
    constexpr data_slice();

    /// Literal bytes constructor.
    /// Integral null terminator is not indexed.
    template <size_type Size>
    constexpr data_slice(const char(&text)[Size]);

    // TODO: restrict to iterator-to-const references.
    /// Byte iterators constructor (casts to uint8_t).
    template <typename Iterator>
    constexpr data_slice(const Iterator& begin, const Iterator& end);

    /// String constructor (casts char to uint8_t).
    constexpr data_slice(const std::string& text);

    /// Byte initializer list constructor.
    constexpr data_slice(std::initializer_list<value_type> bytes);

    /// Methods.
    /// -----------------------------------------------------------------------

    /// Copy data to a vector.
    constexpr std::vector<value_type> to_chunk() const;

    /// Convert data to a string (casts uint8_t to char).
    constexpr std::string to_string() const;

    /// Resize the slice by decrementing the end pointer.
    /// This is the only mutable action that can be taken on the slice.
    /// Returns true if the size was reduced (expansion is not allowed).
    constexpr bool resize(size_t size);

    /// Properties.
    /// -----------------------------------------------------------------------
    constexpr pointer data() const;
    constexpr pointer begin() const;
    constexpr pointer end() const;
    constexpr value_type front() const;
    constexpr value_type back() const;
    constexpr size_type size() const;
    constexpr bool empty() const;

    /// Operators.
    /// -----------------------------------------------------------------------
    ////template<size_type Size>
    ////constexpr operator std_array<value_type, Size>() const;
    ////VCONSTEXPR operator std::vector<value_type>() const;
    constexpr value_type operator[](size_type index) const;

private:
    constexpr data_slice(pointer begin, pointer end, size_type size);

    template <size_type Size, typename Byte>
    static constexpr data_slice from_literal(const Byte(&text)[Size]);

    template <typename Iterator>
    static constexpr data_slice from_iterators(const Iterator& begin,
        const Iterator& end);

    // TODO: not constexpr (reinterpret_cast).
    template <typename Pointer>
    static constexpr data_slice from_size(
        Pointer begin, size_type size);

    template <typename Pointer>
    static constexpr data_slice from_size_(
        Pointer begin, size_type size);

    pointer begin_;
    pointer end_;
    size_type size_;
};

/// Binary operators.
constexpr bool operator==(const data_slice& left, const data_slice& right);
constexpr bool operator!=(const data_slice& left, const data_slice& right);

typedef std::initializer_list<data_slice> data_loaf;

#endif
