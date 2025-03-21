#include "GameManager.h"

namespace SDLFramework {

	GameManager * GameManager::sInstance = nullptr;

	GameManager * GameManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new GameManager();
		}
		return sInstance;
	}

	void GameManager::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void GameManager::Run() {
		while (!mQuit) {
			mTimer->Update();

			while (SDL_PollEvent(&mEvent)) {
				switch (mEvent.type) {
				case SDL_QUIT:
					mQuit = true;
					break;
				case SDL_MOUSEWHEEL:
					mInputManager->MouseWheel(mEvent.wheel.y);
					break;
				}
			}

			if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE) {
				Update();
				LateUpdate();
				Render();
				mTimer->Reset();
			}
		}
	}

	void GameManager::Update() {
		mInputManager->Update();

		mScreenManager->Update();
	}

	void GameManager::LateUpdate() {
		mInputManager->UpdatePrevInput();
		mPhysicsManager->Update();
	}

	void GameManager::Render() {
		mGraphics->ClearBackBuffer();
		mScreenManager->Render();
		mGraphics->Render();
	}

	GameManager::GameManager() : mQuit(false), mEvent(SDL_Event()) {
		Graphics::SetMode(Graphics::RenderMode::SDL);
		mGraphics = Graphics::Instance();

		if (!Graphics::Initialized()) {
			mQuit = true;
		}

		mAssetManager = AssetManager::Instance();
		mInputManager = InputManager::Instance();
		mAudioManager = AudioManager::Instance();
		mTimer = Timer::Instance();
		mScreenManager = ScreenManager::Instance();
		mPhysicsManager = PhysicsManager::Instance();

		mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Friendly, PhysicsManager::CollisionFlags::Hostile | PhysicsManager::CollisionFlags::HostileProjectiles | PhysicsManager::CollisionFlags::Police);
		mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::FriendlyProjectiles, PhysicsManager::CollisionFlags::Hostile);
		mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Hostile, PhysicsManager::CollisionFlags::Friendly | PhysicsManager::CollisionFlags::FriendlyProjectiles | PhysicsManager::CollisionFlags::Police);
		mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::HostileProjectiles, PhysicsManager::CollisionFlags::Friendly); // Spike strips are remote controlled by nearby police cars lol
		mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Police, PhysicsManager::CollisionFlags::Friendly | PhysicsManager::CollisionFlags::Hostile);

		//mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Friendly, PhysicsManager::CollisionFlags::Hostile | PhysicsManager::CollisionFlags::HostileProjectiles | PhysicsManager::CollisionFlags::Police);
		//mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::FriendlyProjectiles, PhysicsManager::CollisionFlags::Hostile);
		//mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Hostile, PhysicsManager::CollisionFlags::Friendly | PhysicsManager::CollisionFlags::FriendlyProjectiles);
		//mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::HostileProjectiles, PhysicsManager::CollisionFlags::Friendly); // Spike strips are remote controlled by nearby police cars lol
		//mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Police, PhysicsManager::CollisionFlags::Friendly);
	}

	GameManager::~GameManager() {
		ScreenManager::Release();
		mScreenManager = nullptr;

		Timer::Release();
		mTimer = nullptr;

		AudioManager::Release();
		mAudioManager = nullptr;

		InputManager::Release();
		mInputManager = nullptr;

		AssetManager::Release();
		mAssetManager = nullptr;

		PhysicsManager::Release();
		mPhysicsManager = nullptr;

		Graphics::Release();
		mGraphics = nullptr;

		// Quit SDL subsystems
		SDL_Quit();
	}
}