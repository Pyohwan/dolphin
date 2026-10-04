// Copyright 2018 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <atomic>

enum class WindowSystemType
{
  Headless,
  Windows,
  MacOS,
  Android,
  X11,
  Wayland,
  FBDev,
  Haiku,
#ifdef __LIBRETRO__
  Libretro,
#endif
};

struct WindowSystemInfo
{
  WindowSystemInfo() = default;
  WindowSystemInfo(WindowSystemType type_, void* display_connection_, void* render_window_,
                   void* render_surface_)
      : type(type_), display_connection(display_connection_), render_window(render_window_),
        render_surface(render_surface_)
  {
  }

  // Window system type. Determines which GL context or Vulkan WSI is used.
  WindowSystemType type = WindowSystemType::Headless;

  // Connection to a display server. This is used on X11 and Wayland platforms.
  void* display_connection = nullptr;

  // Render window. This is a pointer to the native window handle, which depends
  // on the platform. e.g. HWND for Windows, Window for X11. If the surface is
  // set to nullptr, the video backend will run in headless mode.
  void* render_window = nullptr;

  // Render surface. Depending on the host platform, this may differ from the window.
  // This is kept separate as input may require a different handle to rendering, and
  // during video backend startup the surface pointer may change (MoltenVK).
  void* render_surface = nullptr;

  // Scale of the render surface. For hidpi systems, this will be >1.
  float render_surface_scale = 1.0f;
};

namespace Common
{
// Last render window size reported by the host frontend, in pixels (0 if unknown). Window
// surfaces that have no size of their own (Wayland) use it; it is also needed before the
// presenter exists (TOPST D3-G).
inline std::atomic<int> g_host_surface_width{0};
inline std::atomic<int> g_host_surface_height{0};

inline void SetHostSurfaceSize(int width, int height)
{
  g_host_surface_width.store(width, std::memory_order_relaxed);
  g_host_surface_height.store(height, std::memory_order_relaxed);
}
inline int GetHostSurfaceWidth()
{
  return g_host_surface_width.load(std::memory_order_relaxed);
}
inline int GetHostSurfaceHeight()
{
  return g_host_surface_height.load(std::memory_order_relaxed);
}
}  // namespace Common
