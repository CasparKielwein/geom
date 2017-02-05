#include <benchmark/benchmark.h>

#include "collection.h"

#include <random>
#include <algorithm>

#define __assume(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)


struct tag {
	long t = 0;
	long x = 0;
	long y = 0;

	bool operator==(tag o) const {
		return t == o.t;
	}
};

using tagged_vec3d = geom::object<Eigen::Vector3d,tag>;
using tagged_vec4d = geom::object<Eigen::Vector4d,tag>;
using tagged_vec3f = geom::object<Eigen::Vector3f,tag>;

static void Vector3d(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> d(0, 10000);
	std::vector<Eigen::Vector3d> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return Eigen::Vector3d{d(gen),d(gen),d(gen)};});

	Eigen::Affine3d m{};
	m = Eigen::AngleAxisd(0.9, Eigen::Vector3d::UnitZ())
	  * Eigen::AngleAxisd(1.234, Eigen::Vector3d::UnitY())
	  * Eigen::AngleAxisd(-43, Eigen::Vector3d::UnitZ())
	  * Eigen::Translation3d(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&&x : a) {
			x = m*x;
		}

		benchmark::DoNotOptimize(a);
	}
}

static void Vector4d(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> d(0, 10000);
	std::vector<Eigen::Vector4d> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return Eigen::Vector4d{d(gen),d(gen),d(gen),d(gen)};});

	Eigen::Affine3d m{};
	m = Eigen::AngleAxisd(0.9, Eigen::Vector3d::UnitZ())
	  * Eigen::AngleAxisd(1.234, Eigen::Vector3d::UnitY())
	  * Eigen::AngleAxisd(-43, Eigen::Vector3d::UnitZ())
	  * Eigen::Translation3d(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&&x : a) {
			x = m*x;
		}

		benchmark::DoNotOptimize(a);
	}
}

static void Tagged3d(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> d(0, 10000);
	std::vector<tagged_vec3d> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return tagged_vec3d{Eigen::Vector3d{d(gen),d(gen),d(gen)},tag{0}}; });

	Eigen::Affine3d m{};
	m = Eigen::AngleAxisd(0.9, Eigen::Vector3d::UnitZ())
	  * Eigen::AngleAxisd(1.234, Eigen::Vector3d::UnitY())
	  * Eigen::AngleAxisd(-43, Eigen::Vector3d::UnitZ())
	  * Eigen::Translation3d(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&&x : a) {
			x.point = m*x.point;
		}

		benchmark::DoNotOptimize(a);
	}
}

static void Tagged3f(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> d(0, 10000);
	std::vector<tagged_vec3f> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return tagged_vec3f{Eigen::Vector3f{d(gen),d(gen),d(gen)},tag{0}}; });

	Eigen::Affine3f m{};
	m = Eigen::AngleAxisf(0.9, Eigen::Vector3f::UnitZ())
	  * Eigen::AngleAxisf(1.234, Eigen::Vector3f::UnitY())
	  * Eigen::AngleAxisf(-43, Eigen::Vector3f::UnitZ())
	  * Eigen::Translation3f(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&&x : a) {
			x.point = m*x.point;
		}

		benchmark::DoNotOptimize(a);
	}
}


static void Vector3f(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> d(0, 10000);
	std::vector<Eigen::Vector3f> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return Eigen::Vector3f{d(gen),d(gen),d(gen)};});

	Eigen::Affine3f m
			= Eigen::AngleAxisf(0.9, Eigen::Vector3f::UnitZ())
			  * Eigen::AngleAxisf(1.234, Eigen::Vector3f::UnitY())
			  * Eigen::AngleAxisf(-43, Eigen::Vector3f::UnitZ())
			  * Eigen::Translation3f(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&&x : a) {
			x = m*x;
		}

		benchmark::DoNotOptimize(a);
	}
}

static constexpr auto storage_order = Eigen::RowMajor;

//static void Matrix3d(benchmark::State& state) {
//
//	std::random_device rd{};
//	std::mt19937 gen(rd());
//
//	std::uniform_real_distribution<> d(0, 10000);
//	Eigen::Matrix<double, 3, Eigen::Dynamic, storage_order> a
//		(Eigen::Matrix<double, 3, Eigen::Dynamic, storage_order>::Random(3,state.range(0)));
//
//	Eigen::Affine3d m{};
//	m = Eigen::AngleAxisd(0.9, Eigen::Vector3d::UnitZ())
//	  * Eigen::AngleAxisd(1.234, Eigen::Vector3d::UnitY())
//	  * Eigen::AngleAxisd(-43, Eigen::Vector3d::UnitZ())
//	  * Eigen::Translation3d(1.,1.,2.);
//
//	while (state.KeepRunning()) {
//
//		a = m * a;
//
//		benchmark::DoNotOptimize(a);
//	}
//}
//
//static void Matrix3f(benchmark::State& state) {
//
//	std::random_device rd{};
//	std::mt19937 gen(rd());
//
//	std::uniform_real_distribution<float> d(0, 10000);
//	Eigen::Matrix<float, 3, Eigen::Dynamic, storage_order> a
//		(Eigen::Matrix<float, 3, Eigen::Dynamic, storage_order>::Random(3,state.range(0)));
//
//	Eigen::Affine3f m{};
//	m = Eigen::AngleAxisf(0.9, Eigen::Vector3f::UnitZ())
//	  * Eigen::AngleAxisf(1.234, Eigen::Vector3f::UnitY())
//	  * Eigen::AngleAxisf(-43, Eigen::Vector3f::UnitZ())
//	  * Eigen::Translation3f(1.,1.,2.);
//
//	while (state.KeepRunning()) {
//
//		a = m * a;
//
//		benchmark::DoNotOptimize(a);
//	}
//}

static void geom3dint(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<> d(0, 10000);
	geom::collection<tagged_vec3d> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return tagged_vec3d{Eigen::Vector3d{d(gen),d(gen),d(gen)},tag{0}}; });

	Eigen::Affine3d m{};
	m = Eigen::AngleAxisd(0.9, Eigen::Vector3d::UnitZ())
	  * Eigen::AngleAxisd(1.234, Eigen::Vector3d::UnitY())
	  * Eigen::AngleAxisd(-43, Eigen::Vector3d::UnitZ())
	  * Eigen::Translation3d(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&& x : a.points()) {
			x = m*x;
		}
		benchmark::DoNotOptimize(a);
	}
}

static void geom3fint(benchmark::State& state) {

	std::random_device rd{};
	std::mt19937 gen(rd());

	std::uniform_real_distribution<float> d(0, 10000);
	geom::collection<tagged_vec3f> a(state.range(0));

	std::generate(a.begin(), a.end(), [&]() {return tagged_vec3f{Eigen::Vector3f{d(gen),d(gen),d(gen)},tag{0}}; });

	Eigen::Affine3f m{};
	m = Eigen::AngleAxisf(0.9, Eigen::Vector3f::UnitZ())
	  * Eigen::AngleAxisf(1.234, Eigen::Vector3f::UnitY())
	  * Eigen::AngleAxisf(-43, Eigen::Vector3f::UnitZ())
	  * Eigen::Translation3f(1.,1.,2.);

	while (state.KeepRunning()) {

		for (auto&& x : a.points()) {
			x = m*x;
		}
		benchmark::DoNotOptimize(a);
	}
}

static constexpr int benchmark_size = 8<<12;


BENCHMARK(Vector3d)->RangeMultiplier(2)->Range(64, benchmark_size);
//BENCHMARK(Vector4d)->RangeMultiplier(2)->Range(64, benchmark_size);
BENCHMARK(Vector3f)->RangeMultiplier(2)->Range(64, benchmark_size);
//BENCHMARK(Matrix3d)->RangeMultiplier(2)->Range(64, benchmark_size);
//BENCHMARK(Matrix3f)->RangeMultiplier(2)->Range(64, benchmark_size);
BENCHMARK(Tagged3d)->RangeMultiplier(2)->Range(64, benchmark_size);
BENCHMARK(Tagged3f)->RangeMultiplier(2)->Range(64, benchmark_size);
BENCHMARK(geom3dint)->RangeMultiplier(2)->Range(64, benchmark_size);
BENCHMARK(geom3fint)->RangeMultiplier(2)->Range(64, benchmark_size);

BENCHMARK_MAIN()
