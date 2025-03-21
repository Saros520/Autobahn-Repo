#include "Texture.h"

namespace SDLFramework {

	Texture::Texture(std::string filename, bool managed) {
		mGraphics = Graphics::Instance();
		mTex = AssetManager::Instance()->GetTexture(filename, managed);

		if (mTex == nullptr) {
			std::cerr << "Unable to load texture from file: " << filename << " SDL Error: " << SDL_GetError() << std::endl;
		}

		SDL_QueryTexture(mTex, nullptr, nullptr, &mWidth, &mHeight);

		mClipped = false;
		mSourceRect = SDL_Rect();
		mDestinationRect.w = mWidth;
		mDestinationRect.h = mHeight;
		mFlip = SDL_FLIP_NONE;
	}

	Texture::Texture(std::string filename, int x, int y, int w, int h, bool managed) {
		mGraphics = Graphics::Instance();
		mTex = AssetManager::Instance()->GetTexture(filename, managed);

		if (mTex == nullptr) {
			std::cerr << "Unable to load texture from file: " << filename << " SDL Error: " << SDL_GetError() << std::endl;
		}

		mWidth = w;
		mHeight = h;
		mClipped = true;
		mDestinationRect.w = mWidth;
		mDestinationRect.h = mHeight;

		mSourceRect.x = x;
		mSourceRect.y = y;
		mSourceRect.w = mWidth;
		mSourceRect.h = mHeight;
		mFlip = SDL_FLIP_NONE;
	}

	Texture::Texture(std::string text, std::string fontPath, int size, SDL_Color color, bool managed) {
		mGraphics = Graphics::Instance();
		mTex = AssetManager::Instance()->GetText(text, fontPath, size, color, managed);

		if (mTex == nullptr) {
			std::cerr << "Unable to create texture from text: " << text << " TTF Error: " << TTF_GetError() << std::endl;
		}

		mClipped = false;
		SDL_QueryTexture(mTex, nullptr, nullptr, &mWidth, &mHeight);
		mSourceRect = SDL_Rect();
		mDestinationRect.w = mWidth;
		mDestinationRect.h = mHeight;
	}

	Texture::~Texture() {
		AssetManager::Instance()->DestroyTexture(mTex);
		mTex = nullptr;
		mGraphics = nullptr;
	}

	Vector2 Texture::ScaledDimensions() {
		Vector2 scaledDimensions = Scale();
		scaledDimensions.x *= mWidth;
		scaledDimensions.y *= mHeight;
		return scaledDimensions;
	}

	void Texture::SetSourceRect(SDL_Rect* sourceRect) {
		mSourceRect = *sourceRect;
	}

	void Texture::Render() {
		UpdateDstRect();
		mGraphics->DrawTexture(mTex, mClipped ? &mSourceRect : nullptr, &mDestinationRect, Rotation(World), mFlip);
	}

	void Texture::UpdateDstRect() {
		Vector2 pos = Position(World);
		Vector2 scale = Scale(World);
		mDestinationRect.x = static_cast<int>(pos.x - mWidth * scale.x * 0.5f);
		mDestinationRect.y = static_cast<int>(pos.y - mHeight * scale.y * 0.5f);
		mDestinationRect.w = static_cast<int>(mWidth * scale.x);
		mDestinationRect.h = static_cast<int>(mHeight * scale.y);
	}

	void Texture::Flip(bool hFlip, bool vFlip) {
		if (hFlip && vFlip) {
			mFlip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		else if (hFlip) {
			mFlip = SDL_FLIP_HORIZONTAL;
		}
		else if (vFlip) {
			mFlip = SDL_FLIP_VERTICAL;
		}
		else {
			mFlip = SDL_FLIP_NONE;
		}
	}

	void Texture::SetColor(Uint8 red, Uint8 green, Uint8 blue) {
		SDL_SetTextureColorMod(mTex, red, green, blue);
	}

	void Texture::Alpha(Uint8 alpha) {
		SDL_SetTextureAlphaMod(mTex, alpha);
	}

	bool Texture::IsValid() const {
		return mTex != nullptr;
	}

	int Texture::GetWidth() const {
		return mWidth;
	}

	int Texture::GetHeight() const {
		return mHeight;
	}

}

