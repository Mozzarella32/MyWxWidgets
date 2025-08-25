#include "WxTextureUtil.hpp"
#include <new>

Texture textureFromWxImage(const wxImage& image, Texture::Descriptor desc) {

  desc.Internal_Format = image.HasAlpha() ? GL_RGBA : GL_RGB;

	size_t size = image.GetWidth() * image.GetHeight();

  std::vector<unsigned char> bytes;
	if (!image.HasAlpha()) {
  	bytes = std::vector<unsigned char>(image.GetData(), image.GetData() + 3 * size);
    return Texture(image.GetWidth(), image.GetHeight(), bytes.data(), desc);
	}
  auto Data = image.GetData();
  auto Alpha = image.GetAlpha();
  bytes.reserve(size * 4);
  for (size_t i = 0; i < size; i++) {
  	bytes.push_back(Data[3 * i + 0]);
  	bytes.push_back(Data[3 * i + 1]);
  	bytes.push_back(Data[3 * i + 2]);
  	bytes.push_back(Alpha[i]);
  }
  return Texture(image.GetWidth(), image.GetHeight(), bytes.data(), desc);
}

wxImage textureToWxImage(const Texture& texture) {
    
	bool hasAlpha = (texture.desc.Internal_Format == GL_RGBA);

  const auto& width = texture.GetWidth();
  const auto& height = texture.GetHeight();

	size_t pixelSize = hasAlpha ? 4 : 3;
	std::vector<unsigned char> pixels(width * height * pixelSize);
	GLCALL(glGetTexImage(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels.data()));

	if (!hasAlpha) {
    // Create ownership
		unsigned char* data = static_cast<unsigned char*>(malloc(width * height * 3));
		std::memcpy(data, pixels.data(), width * height * 3);
		return wxImage(width, height, data, false);
	}
	else {
    // Create ownership
		unsigned char* imageData = static_cast<unsigned char*>(malloc(width * height * 3));
		unsigned char* alphaData = static_cast<unsigned char*>(malloc(width * height));

    if(imageData == nullptr || alphaData == nullptr) {
      throw std::bad_alloc();
    }

		for (int i = 0; i < width * height; ++i) {
			imageData[i * 3 + 0] = pixels[i * pixelSize + 0];
			imageData[i * 3 + 1] = pixels[i * pixelSize + 1];
			imageData[i * 3 + 2] = pixels[i * pixelSize + 2];
			alphaData[i] = pixels[i * pixelSize + 3];
		}
		return wxImage(width, height, imageData, alphaData, false);
	}
}
