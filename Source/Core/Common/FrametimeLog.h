// TOPST D3-G benchmark: FRAMETIME_LOG=<csv> writes one "usec" line (monotonic timestamp) per
// presented frame, same as the RetroArch video_driver_frame() hook's first column. Off unless the
// env var is set. Compiled out in the libretro core (RetroArch logs frames itself).
#pragma once
#include <cstdio>
#include <cstdlib>
#include <ctime>

#if defined(__LIBRETRO__) || defined(LIBRETRO) || !defined(__linux__)
static inline void FrametimeLogPresent() {}
#else
static inline void FrametimeLogPresent() {
	static FILE *f = nullptr;
	static bool init = false;
	if (!init) {
		init = true;
		const char *path = std::getenv("FRAMETIME_LOG");
		if (path && path[0]) {
			f = std::fopen(path, "w");
			if (f)
				std::setvbuf(f, nullptr, _IOLBF, 0);
		}
	}
	if (!f)
		return;
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	std::fprintf(f, "%lld\n", (long long)ts.tv_sec * 1000000LL + ts.tv_nsec / 1000);
}
#endif
