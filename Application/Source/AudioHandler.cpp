#include "AudioHandler.h"

bool AudioHandler::loaded = false;
ISoundSource* AudioHandler::soundSource[SOUND_COUNT];
std::string AudioHandler::fileLocation[SOUND_COUNT];
ISoundEngine* AudioHandler::engine;

void AudioHandler::load() {
	if (!loaded) {

		// start the sound engine with default parameters
		engine = createIrrKlangDevice();
		if (!engine) {
			loaded = false;
			return;
		}
		//File Name Assigning, Sound Source Loading
		//Added ESM_AUTO_DETECT, true) to PRE-LOAD Sounds, preventing lag.
		fileLocation[CAR_CRASH] = "Media\\car_crash.mp3";
		soundSource[CAR_CRASH] = engine->addSoundSourceFromFile(getFileLocation(CAR_CRASH), ESM_AUTO_DETECT, true);

		fileLocation[GUN_PISTOL_EMPTY] = "Media\\Pistol\\EmptyMag.mp3";
		soundSource[GUN_PISTOL_EMPTY] = engine->addSoundSourceFromFile(getFileLocation(GUN_PISTOL_EMPTY), ESM_AUTO_DETECT, true);

		fileLocation[GUN_PISTOL_SHOOT] = "Media\\Pistol\\Shot.mp3";
		soundSource[GUN_PISTOL_SHOOT] = engine->addSoundSourceFromFile(getFileLocation(GUN_PISTOL_SHOOT), ESM_AUTO_DETECT, true);

		fileLocation[GUN_PISTOL_RELOAD] = "Media\\Pistol\\Reload.wav";
		soundSource[GUN_PISTOL_RELOAD] = engine->addSoundSourceFromFile(getFileLocation(GUN_PISTOL_RELOAD), ESM_AUTO_DETECT, true);

		fileLocation[GUN_SILENCER_SHOOT] = "Media\\silencer_fire.mp3";
		soundSource[GUN_SILENCER_SHOOT] = engine->addSoundSourceFromFile(getFileLocation(GUN_SILENCER_SHOOT), ESM_AUTO_DETECT, true);

		fileLocation[FIRE_SOUND] = "Media\\fire.mp3";
		soundSource[FIRE_SOUND] = engine->addSoundSourceFromFile(getFileLocation(FIRE_SOUND), ESM_AUTO_DETECT, true);

		fileLocation[PORTAL] = "Media\\portal.mp3";
		soundSource[PORTAL] = engine->addSoundSourceFromFile(getFileLocation(PORTAL), ESM_AUTO_DETECT, true);

		fileLocation[DOOR] = "Media\\Door.mp3";
		soundSource[DOOR] = engine->addSoundSourceFromFile(getFileLocation(DOOR), ESM_AUTO_DETECT, true);

		fileLocation[FIRE_EXTINGUISH] = "Media\\fireextinguish.mp3";
		soundSource[FIRE_EXTINGUISH] = engine->addSoundSourceFromFile(getFileLocation(FIRE_EXTINGUISH), ESM_AUTO_DETECT, true);

		loaded = true;
	}
}

ISoundEngine* AudioHandler::getEngine() {
	return engine;
}

vec3df AudioHandler::to_vec3df(Vector3 vec) {
	return vec3df(vec.x, vec.y, vec.z);
}

bool AudioHandler::isLoaded() {
	return loaded;
}

const char* AudioHandler::getFileLocation(SOUNDTYPE type) {
	return fileLocation[type].c_str();
}

ISoundSource* AudioHandler::getSoundSource(SOUNDTYPE type) {
	return soundSource[type];
}

void AudioHandler::cleanup() {
	engine->removeAllSoundSources();
	engine->drop();
}

