#pragma once

#include <string>

namespace aie {

	// a class for wrapping up an opengl texture image
	class Texture {
	public:

		enum Format : unsigned int {
			RED = 1,
			RG,
			RGB,
			RGBA
		};
		enum TextureType : unsigned int {
			DIFFUSE = 0,
			SPECULAR,
			NORMAL,
			GLOW,
			ENUM_SIZE
		};
		struct TextureName
		{
			static const char* GetName(int nameNumber)
			{
				const char* name;
				switch (nameNumber)
				{
				case 0:
					name = "diffuseMap";
					break;
				case 1:
					name = "normMap";
					break;
				case 2:
					name = "specMap";
					break;
				case 3:
					name = "glossMap";
					break;
				default:
					name = "";
				}
				return name;
			}
		};

		Texture();
		Texture(const char* filename, TextureType texturetype, std::string name);
		Texture(unsigned int width, unsigned int height, Format format, std::string name, TextureType texturetype, unsigned char* pixels = nullptr);
		virtual ~Texture();

		// load a jpg, bmp, png or tga
		bool load(const char* filename);

		// creates a texture that can be filled in with pixels
		void create(unsigned int width, unsigned int height, Format format, unsigned char* pixels = nullptr);
		void SetToHandle(unsigned int handle, unsigned int width, unsigned int height);
		// returns the filename or "none" if not loaded from a file
		const std::string& getFilename() const { return m_filename; }
		const std::string& getName() const { return m_name; }
		TextureType getFileType() const { return m_textureType; }
		// returns the opengl texture handle
		unsigned int getHandle() const { return m_glHandle; }

		unsigned int getWidth() const { return m_width; }
		unsigned int getHeight() const { return m_height; }
		unsigned int getFormat() const { return m_format; }
		const unsigned char* getPixels() const { return m_loadedPixels; }

	protected:

		std::string		m_filename;
		std::string		m_name;
		TextureType		m_textureType;
		unsigned int	m_width;
		unsigned int	m_height;
		unsigned int	m_glHandle;
		unsigned int	m_format;
		unsigned char*	m_loadedPixels;
	};

} // namespace aie