// Copyright 2019 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Common/GL/GLInterface/EGL.h"

struct wl_egl_window;

// EGL on a Wayland surface handed over by the Qt frontend (TOPST D3-G). Based on the
// EGLWayland code in batocera's dolphin Wayland patch. The surface size comes from the host
// window size (Common::GetHostSurfaceWidth/Height) since a wl_surface has no size of its own.
class GLContextEGLWayland final : public GLContextEGL
{
public:
  ~GLContextEGLWayland() override;

  void Update() override;

protected:
  EGLDisplay OpenEGLDisplay() override;
  EGLNativeWindowType GetEGLNativeWindow(EGLConfig config) override;

private:
  wl_egl_window* m_egl_window = nullptr;
};
