#ifndef ACCEL_H
#define ACCEL_H
#include <ranges>
#include <variant>
#include <string>

namespace accel::inline core
{
	template<class T>
	concept input = std::ranges::range<T> && requires(T input)
	{
		{ input.current() } -> std::same_as<std::ranges::iterator_t<T>>;
	};

	template<class Iterator,class Sentinel>
	struct input_wrapper
	{
		using iterator_type = typename Iterator;
		using sentinel_type = typename Sentinel;

		Iterator _current{};
		Iterator _begin{};
		Sentinel _end{};

		template<std::ranges::range Iterable>
		constexpr input_wrapper(Iterable&& range) 
			noexcept(noexcept(std::ranges::begin(range) && noexcept(std::ranges::end(range))))
			: _current	{ std::ranges::begin(range) }
			, _begin	{ std::ranges::begin(range) }
			, _end		{ std::ranges::end(range)	}
		{}

		template<input Input>
		constexpr input_wrapper(Input&& input)
			noexcept(noexcept(input.current()) && noexcept(std::ranges::begin(input) && noexcept(std::ranges::end(input))))
			: _current	{ input.current()			}
			, _begin	{ std::ranges::begin(input) }
			, _end		{ std::ranges::end(input)	}
		{}

		template<class T1,class T2>
		constexpr input_wrapper(T1&& current, T1&& begin, T2&& end)
			noexcept(std::is_nothrow_convertible_v<T1,Iterator> && std::is_nothrow_convertible_v<T2, Sentinel>)
			: _current	{ std::forward<T1>(current)	}
			, _begin	{ std::forward<T1>(begin)	}
			, _end		{ std::forward<T2>(end)		}
		{}

		constexpr Iterator current() const noexcept
		{
			return _current;
		}

		constexpr Iterator begin() const noexcept
		{
			return _begin;
		}

		constexpr Sentinel end() const noexcept
		{
			return _end;
		}
	};

	template<std::ranges::range Iterable>
	input_wrapper(Iterable&&) -> input_wrapper<std::ranges::iterator_t<Iterable>, std::ranges::sentinel_t<Iterable>>;

	template<class Output, input Input>
	struct success_result
	{
		Output output{};
		Input input{};

		template<class T1, class T2>
		constexpr success_result(T1&& _output, T2&& _input)
			noexcept(std::is_nothrow_convertible_v<T1, Output> && std::is_nothrow_convertible_v<T2, Input>)
			: output	{ std::forward<T1>(_output) }
			, input		{ std::forward<T2>(_input)	}
		{}
	};

	template<class T1, class T2>
	success_result(T1&&, T2&&) -> success_result<T1, T2>;

	template<input Input>
	struct failure_result
	{
		Input input{};
		std::string message{};
	};

	template<class T1>
	failure_result(T1&&, std::string) -> failure_result<T1>;

	template<class Output, input Input>
	using result = std::variant< success_result<Output, Input>, failure_result<Input>>;

	template<class Output, input Input>
	constexpr auto success(Input&& input, std::type_identity_t<Output&&> output)
		noexcept(noexcept(result<Output, Input>{ success_result{ std::forward<Output>(output), std::forward<Input>(input) } })) -> result<Output, Input>
	{
		return { success_result{ std::forward<Output>(output), std::forward<Input>(input) } };
	}

	template<class Output, input Input>
	constexpr auto failure(Input && input, std::string const& message)
		noexcept(noexcept(result<Output, Input>{ failure_result<Input>{ std::forward<Input>(input), message }})) -> result<Output, Input>
	{
		return { failure_result<Input>{ std::forward<Input>(input), message } };
	}

	template<class Output, input Input>
	constexpr auto failure(Input && input, std::string && message)
		noexcept(noexcept(result<Output, Input>{ failure_result<Input>{ std::forward<Input>(input), message }})) -> result<Output, Input>
	{
		return { failure_result<Input>{ std::forward<Input>(input), std::move(message) } };
	}

	template<class Output, input Input>
	constexpr bool is_success(result<Output, Input> const& result)
	{
		return std::holds_alternative<success_result<Output, Input>>(result);
	}
}

#endif