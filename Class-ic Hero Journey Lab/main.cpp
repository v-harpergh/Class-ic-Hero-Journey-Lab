/*
	C++ Standard: C++20 (VS 2026)
	Compiler: MSVC
*/
#include <iostream>
#include <cmath>
#include <concepts>
#include <cassert>

#include <random>
#include <string>


// I learned this stuff from prior experience, I can explain it in-person if needed

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

enum class Result : uint8_t { Success, Failure, Neutral };

class Hero {
public:
	 explicit Hero(std::string name) : name_(name), strength_(Random(1.0f, 5.0f)), courage_(Random(0.2f, 0.35f)) {}

	// Public utility stuff

	void AttendTraining(uint32_t hours) noexcept {
		strength_ += static_cast<float>((0.625 * hours));
	}

	void AttendTherapy() noexcept {
		addCourage(Random(1.0f, 2.0f));
	}

	// Properties
	[[nodiscard]]
	std::string_view getName() const noexcept {
		return name_;
	}

	[[nodiscard]]
	float getStrength() const noexcept {
		return strength_;
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

	Result Start(Hero& hero) {
		Result status = Start_(hero);

		std::cout << hero.getName() << " completed a quest, and... ";

		switch (status) {

		case Result::Success: {
			std::cout << "WON!!!\n";
			break;
		}

		case Result::Neutral: {
			std::cout << "resulted in a draw.\n";
			break;
		}

		case Result::Failure: {
			std::cout << "failed.\n";
			break;
		}

		}

		return status;
	}
private:
	int difficulty_{};

	Result Start_(Hero& hero) {
		if (difficulty_ > hero.getStrength()) {
			if (Random(0, 7) == 0) {
				hero.addCourage(-Random(0.1f, 0.17f));
				return Result::Neutral;
			} 
			else {
				hero.addCourage(-Random(0.2f, 0.35f));
				return Result::Failure;
			}
		}
		else if (difficulty_ == hero.getStrength()) {
			hero.addCourage(-Random(0.04f, 0.125f));
			return Result::Neutral;
		}
		else {
			hero.addCourage(-Random(0.0f, 0.03f));
			return Result::Success;
		}
	}
};

Result sendOnQuest(Hero& hero) {
	Quest newQuest = Quest(Random(1, 5));

	return newQuest.Start(hero);
}

void runTests() {

	{
		Hero dummy = Hero("Dummy");
		float courage = dummy.getCourage();

		dummy.AttendTherapy();

		assert(dummy.getCourage() > courage);
	}

	{
		Hero dummy("Dummy");
		float strength = dummy.getStrength();

		dummy.AttendTraining(2);

		assert(dummy.getStrength() > strength);
	}

	{
		Hero dummy("Dummy");

		assert(dummy.getName() == "Dummy");
	}

	{
		Hero dummy("Dummy");

		dummy.setCourage(0.5f);

		assert(dummy.getCourage() == 0.5f);
	}

}

int main() {
	runTests();

	Hero bob("Bob");
	Hero alice("Alice");
	Hero steve("Steve");

	sendOnQuest(bob);
	sendOnQuest(alice);
	sendOnQuest(steve);

	bob.AttendTraining(2);
	alice.AttendTraining(1);
	bob.AttendTherapy();

	sendOnQuest(bob);
	sendOnQuest(alice);
	sendOnQuest(steve);
}
