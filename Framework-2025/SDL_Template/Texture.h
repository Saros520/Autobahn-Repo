#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "GameEntity.h"
#include "AssetManager.h"

namespace SDLFramework {

	class Texture : public GameEntity {
	protected:
		SDL_Texture * mTex;
		Graphics * mGraphics;

		int mWidth;
		int mHeight;

		bool mClipped;
		SDL_Rect mSourceRect;
		SDL_Rect mDestinationRect;

		SDL_RendererFlip mFlip;

	public:
		Texture(std::string filename, bool managed = true);
		Texture(std::string filename, int x, int y, int w, int h, bool managed = true);
		Texture(std::string text, std::string fontPath, int size, SDL_Color color, bool managed = true);
		virtual ~Texture();

		void Alpha(Uint8 alpha);
		Vector2 ScaledDimensions();

		void SetSourceRect(SDL_Rect * sourceRect);

		void Flip(bool hFlip, bool vFLip);

		void SetColor(Uint8 red, Uint8 green, Uint8 blue);

		bool IsValid() const;

		virtual void Render() override;

		int GetWidth() const;
		int GetHeight() const;

	protected:
		void UpdateDstRect();
	};
}
#endif