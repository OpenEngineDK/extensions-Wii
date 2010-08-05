# Create the extension library
ADD_LIBRARY(Extensions_Wii
  Display/WiiEnvironment.h
  Display/WiiFrame.h
  Display/WiiFrame.cpp
  Display/WiiCanvas.h
  Display/WiiCanvas.cpp
  Renderers/WiiRenderer.h
  Renderers/WiiRenderer.cpp
  Renderers/WiiRenderingView.h
  Renderers/WiiRenderingView.cpp
)

TARGET_LINK_LIBRARIES(Extensions_Wii
  OpenEngine_Display
  OpenEngine_Renderers
  ${WIIUSE_LIBRARIES}
  ${OGC_LIBRARIES}
)

