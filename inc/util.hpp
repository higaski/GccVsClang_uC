#pragma once

#include <gsl/span>
#include <optional>

/// Is specialization
template<typename, template<typename...> typename>
struct is_specialization : std::false_type {};

/// True if T<Ts...> is a specialized type of T
///
/// \tparam Ts...   Types of T
/// \tparam T       Type to specialize from
template<typename... Ts, template<typename...> typename T>
struct is_specialization<T<Ts...>, T> : std::true_type {};

// Helper variable template
template<typename, template<typename...> typename>
inline constexpr bool is_specialization_v{false};
template<typename... Ts, template<typename...> typename T>
inline constexpr bool is_specialization_v<T<Ts...>, T>{true};

namespace detail {

/// Visit implementation
///
/// \tparam F       Type of callable object to be invoked
/// \tparam T       Type of tuple
/// \tparam U       Type of span
/// \tparam J       Size of span
/// \param  f       Callable object
/// \param  t       Tuple
/// \param  s       Span of tuple indexes
/// \return Value returned by F
template<size_t I, typename F, typename T, typename U, ptrdiff_t J = -1>
constexpr decltype(auto) visit_impl(F&& f, T&& t, gsl::span<U, J> s) {
  using R = std::invoke_result_t<F, T>;

  if constexpr (I > 0) {
    if (s[0] == I - 1) {
      auto i{s.size()};
      // Still indexes in the span
      if (i > 1) {
        // Increasing depth is only possible, if next element is a tuple as well
        if constexpr (is_specialization_v<
                          std::tuple_element_t<I - 1, std::decay_t<T>>,
                          std::pair> ||
                      is_specialization_v<
                          std::tuple_element_t<I - 1, std::decay_t<T>>,
                          std::tuple>) {
          return visit_impl<
              std::tuple_size_v<std::tuple_element_t<I - 1, std::decay_t<T>>>>(
              std::forward<F>(f),
              std::get<I - 1>(std::forward<T>(t)),
              s.last(i - 1));
        }
        // Not a tuple
        else {
          if constexpr (std::is_same_v<R, void>)
            return;
          else
            return std::optional<R>{};
        }
      }
      // Last index of span, invoke callable object
      else {
        if constexpr (std::is_same_v<R, void>)
          return f(std::get<I - 1>(std::forward<T>(t)));
        else
          return std::optional<R>{f(std::get<I - 1>(std::forward<T>(t)))};
      }
    }
    // Index not found
    else
      return visit_impl<I - 1>(std::forward<F>(f), std::forward<T>(t), s);
  }
  // No more elements
  else {
    if constexpr (std::is_same_v<R, void>)
      return;
    else
      return std::optional<R>{};
  }
}

}  // namespace detail

/// Applies a callable object to a tuple element
///
/// \tparam F       Type of callable object to be invoked
/// \tparam T       Type of tuple
/// \tparam U       Type of span
/// \tparam J       Size of span
/// \param  f       Callable object
/// \param  t       Tuple
/// \param  s       Span of tuple indexes
/// \return Value returned by F
template<typename F, typename T, typename U, ptrdiff_t J>
constexpr decltype(auto) visit(F&& f, T&& t, gsl::span<U, J> s) {
  return detail::visit_impl<std::tuple_size_v<std::decay_t<T>>>(
      std::forward<F>(f), std::forward<T>(t), s);
}
