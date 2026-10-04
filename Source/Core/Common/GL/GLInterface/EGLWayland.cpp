// Copyright 2019 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/GL/GLInterface/EGLWayland.h"

#include <EGL/eglext.h>
#include <wayland-egl.h>

#include "Common/WindowSystemInfo.h"

static void GetHostSize(int* width, int* height)
{
  *width = Common::GetHostSurfaceWidth();
  *height = Common::GetHostSurfaceHeight();
  if (*width <= 0 || *height <= 0)
  {
    *width = 1280;
    *height = 720;
  }
}

GLContextEGLWayland::~GLContextEGLWayland()
{
  // The context and EGL surface must be destroyed before the wl_egl_window.
  DestroyWindowSurface();
  DestroyContext();
  if (m_egl_window)
    wl_egl_window_destroy(m_egl_window);
}

void GLContextEGLWayland::Update()
{
  if (!m_egl_window)
    return;

  int width, height;
  GetHostSize(&width, &height);
  if (static_cast<u32>(width) == m_backbuffer_width &&
      static_cast<u32>(height) == m_backbuffer_height)
  {
    return;
  }

  wl_egl_window_resize(m_egl_window, width, height, 0, 0);
  m_backbuffer_width = width;
  m_backbuffer_height = height;
}

EGLDisplay GLContextEGLWayland::OpenEGLDisplay()
{
  return eglGetPlatformDisplay(EGL_PLATFORM_WAYLAND_KHR, m_wsi.display_connection, nullptr);
}

EGLNativeWindowType GLContextEGLWayland::GetEGLNativeWindow(EGLConfig config)
{
  if (m_egl_window)
  {
    wl_egl_window_destroy(m_egl_window);
    m_egl_window = nullptr;
  }

  int width, height;
  GetHostSize(&width, &height);
  m_egl_window = wl_egl_window_create(static_cast<wl_surface*>(m_wsi.render_surface), width, height);
  if (!m_egl_window)
    return {};

  m_backbuffer_width = width;
  m_backbuffer_height = height;
  return reinterpret_cast<EGLNativeWindowType>(m_egl_window);
}
