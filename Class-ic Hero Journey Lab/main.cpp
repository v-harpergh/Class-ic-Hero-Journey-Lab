/*
	C++ Standard: C++20 (VS 2026)
	Compiler: MSVC
*/
#include <iostream>
#include <cmath>
#include <concepts>

#include <random>
#include <string>

std::random_device Device{};
std::mt19937 MtGen(Device());

template<typename T>
concept BasicNumber = std::is_integral_v<T> || std::is_floating_point_v<T>;

template<BasicNumber T>
T Random(T min, T max) {
	if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(min, max);
		return dist(MtGen);
	}
	else if constexpr (std::is_floating_point_v<T>) {
		std::uniform_real_distribution<T> dist(min, max);
		return dist(MtGen);
	}
}

class Hero {
public:
	 explicit Hero(std::string name) : name_(name), strength_(Random(1.0f, 5.0f)), courage_(Random(0.2f, 0.35f)) {}

	// Public utility stuff

	void AttendTraining(uint32_t hours) noexcept {
		strength_ += static_cast<float>((0.625 * hours));
	}

	void AttendTherapy() noexcept {
		courage_ += Random(1.0f, 2.0f);
	}

	// Properties
	[[nodiscard]]
	std::string_view getName() const noexcept {
		return name_;
	}

	[[nodiscard]]
	float getRawStrength() const noexcept {
		return strength_;
	}

	[[nodiscard]]
	float getStrength() const noexcept {
		return strength_ * (1 / courage_);
	}

	[[nodiscard]]
	float getCourage() const noexcept {
		return courage_;
	}

	void setCourage(float courage) noexcept {
		courage_ = std::clamp(courage, 0.0f, 1.0f);
	}

	void addCourage(float value) noexcept {
		setCourage(getCourage() + value);
	}
private:
	const std::string name_;
	float strength_;
	float courage_;
};

class Quest {
public:
	explicit Quest(int difficulty) : difficulty_(difficulty + Random(0, 1)) {}

	enum class Result: uint8_t { Success, Failure, Neutral };

	Result Start(Hero& hero) {
		Result status = Start_(hero);

		std::cout << hero.getName() << " completed a quest, and... ";

		switch (status) {

		case Result::Success: {
			std::cout << "WON!!!";
			break;
		}

		case Result::Neutral: {
			std::cout << "resulted in a draw.";
			break;
		}

		case Result::Failure: {
			std::cout << "failed.";
			break;
		}

		}

		return status;
	}
private:
	int difficulty_{};

	Result Start_(Hero& hero) {
		if (difficulty_ > hero.getStrength()) {
			hero.addCourage(-Random(0.05f, 0.35f));
			return Random(0, 7) == 0 ? Result::Neutral : Result::Failure;
		}
		else if (difficulty_ == hero.getStrength()) {
			hero.addCourage(-Random(0.0f, 0.125f));
			return Result::Neutral;
		}
		else {
			hero.addCourage(Random(0.0f, 0.015f));
			return Result::Success;
		}
	}
};

int main() {
	const Hero bob = Hero("bob");


}
