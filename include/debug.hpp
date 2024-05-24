#pragma once

namespace debug {
bool checkValidationLayerSupport();

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
}; // namespace debug
