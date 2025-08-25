#pragma once

#include "pch.hpp"

extern Texture textureFromWxImage(const wxImage& Image, Texture::Descriptor desc = Texture::DefaultDescriptor);

// assumes the texture to be bound
extern wxImage textureToWxImage(const Texture& texture);
