namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

/**
 * A JSON token iterator.
 *
 * @private This is not intended for external use.
 */
class token_iterator {
public:
  simdjson_really_inline token_iterator() noexcept;
  simdjson_really_inline token_iterator(token_iterator &&other) noexcept;
  simdjson_really_inline token_iterator &operator=(token_iterator &&other) noexcept;
  simdjson_really_inline token_iterator(const token_iterator &other) noexcept = delete;
  simdjson_really_inline token_iterator &operator=(const token_iterator &other) noexcept = delete;

  /**
   * Get the JSON text for a given token (relative).
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param delta The relative position of the token to retrieve. e.g. 0 = current token,
   *              1 = next token, -1 = prev token.
   *
   * TODO consider a string_view, assuming the length will get stripped out by the optimizer when
   * it isn't used ...
   */
  simdjson_really_inline const uint8_t *peek(int32_t delta=0) const noexcept;
  /**
   * Advance to the next token (returning the current one).
   *
   * Does not check or update depth/expect_value. Caller is responsible for that.
   */
  simdjson_really_inline const uint8_t *advance() noexcept;

  /**
   * If children were left partially iterated / unfinished, this will complete the iteration so we
   * are at a comma or end of document/array/object.
   *
   * @precondition The iterator MUST at or above the given depth.
   * @postcondition The iterator is at the given depth.
   */
  simdjson_really_inline void skip_unfinished_children(uint32_t container_depth) noexcept;

  /**
   * Skips a JSON value, whether it is a scalar, array or object.
   */
  simdjson_really_inline void skip_value() noexcept;

  // NOTE: we don't support a full C++ iterator interface, because we expect people to make
  // different calls to advance the iterator based on *their own* state.

  simdjson_really_inline bool operator==(const token_iterator &other) const noexcept;
  simdjson_really_inline bool operator!=(const token_iterator &other) const noexcept;
  simdjson_really_inline bool operator>(const token_iterator &other) const noexcept;
  simdjson_really_inline bool operator>=(const token_iterator &other) const noexcept;
  simdjson_really_inline bool operator<(const token_iterator &other) const noexcept;
  simdjson_really_inline bool operator<=(const token_iterator &other) const noexcept;

private:
  simdjson_really_inline token_iterator(const uint8_t *buf, uint32_t *index, uint32_t depth) noexcept;

  /**
   * Get the JSON text for a given token (relative).
   *
   * This is not null-terminated; it is a view into the JSON.
   *
   * @param delta The relative position of the token to retrieve. e.g. 0 = current token,
   *              1 = next token, -1 = prev token.
   *
   */
  simdjson_really_inline uint32_t peek_index(int32_t delta=0) const noexcept;

  const uint8_t *buf{};
  const uint32_t *index{};
  uint32_t depth{};

  friend class document;
  friend class object;
  friend class array;
  friend class value;
  friend simdjson_really_inline void logger::log_line(const token_iterator &iter, const char *title_prefix, const char *title, std::string_view detail, int delta, int depth_delta) noexcept;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {