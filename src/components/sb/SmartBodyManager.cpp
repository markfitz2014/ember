#include "SmartBodyManager.h"
#include "SmartBodyPathConsts.h"
#include "SmartBodyLocomotion.h"
#include "SmartBodySkeletonMap.h"

#include <OgreSkeletonInstance.h>
#include <OgreEntity.h>
#include <OgreBone.h>

//#define NDEBUG
#include <cassert>


using namespace Ember;
namespace Ember
{

SmartBodyManager::SmartBodyManager(void)
	: mScene(nullptr), mAssetManager(nullptr), mSimulation(nullptr), mProcessor(nullptr), mIsInit(false), mLocomotion(nullptr)
{
}

SmartBodyManager::~SmartBodyManager(void)
{
	delete mLocomotion;
	delete mScene;
}


//public.
void SmartBodyManager::initialize(void)
{
	//Initialization of SmartBody library.
	mScene = SmartBody::SBScene::getScene();
	mScene->start();

	//Set the member parameters.
	mAssetManager = mScene->getAssetManager();
	mSimulation = mScene->getSimulationManager();
	mProcessor = mScene->getBmlProcessor();

	//Set the media path : where to find the assets.
	mScene->setMediaPath(EMBER_SMARTBODY_MEDIAPATH);

	//Add the different asset paths.
	addAssetPaths();

	//Load the different behaviors.
	loadAllBehaviors();

	//Set init boolean to true.
	mIsInit = true;
}

void SmartBodyManager::addAssetPaths(void) 
{
	//Character skeletons.
	mAssetManager->addAssetPath("motion", EMBER_SMARTBODY_ASSETS_SKELETONS);
}

void SmartBodyManager::loadAllBehaviors(void)
{
	/* Should we load the behaviors at the beginning or when they are used for the first time... 
	I think it is better at the beginning because it can take time, and this is not good to have lagging during the simulation.
	*/

	//Locomotion behavior.
	mLocomotion = new SmartBodyLocomotion(EMBER_SMARTBODY_ASSETS_LOCOMOTION, EMBER_SMARTBODY_SKELETON_LOCOMOTION, this);
	mLocomotion->setup();
}

void SmartBodyManager::retargetAllBehaviors(SmartBody::SBCharacter& character)
{
	//Retarget locomotion behavior set.
	mLocomotion->retarget(character);
}

//public.
bool SmartBodyManager::setCorrespondingSkeletonName(std::string& sbName, const std::string& ogreName)
{
	size_t lastSlash = ogreName.rfind('/');

	if (lastSlash + 1 >= ogreName.size())
		return false;

 	sbName = ogreName.substr(lastSlash + 1) + std::string(EMBER_SMARTBODY_SKELETON_EXTENSION);	
 	return true;
}

//public.
bool SmartBodyManager::hasSkeleton(const std::string& skName, bool load)
{
	assert(mIsInit);

	if (mAssetManager->getSkeleton(skName))
	{
		return true;
	}

	else if (load)
	{
		//Try to load the skeleton.
		mAssetManager->loadAsset(skName);
		if (!mAssetManager->getSkeleton(skName))
		{
			return false;
		}

		//If it exists, then try to get the bone mapping.
		SmartBodySkeletonMap map(skName);
		if (map.exists())
		{
			//Finally, map the skeleton.
			map.setMap(this);
		}

		return true;
	}

	return false;
}

//public.
SmartBody::SBCharacter* SmartBodyManager::createCharacter(const Ogre::Entity& entity, const std::string& group, const std::string& sbSkName)
{
	assert(mIsInit);

	//Create SB character.
	SmartBody::SBCharacter *character = mScene->createCharacter(entity.getName(), group);

	//Create SB skeleton.
	SmartBody::SBSkeleton *skeleton = mScene->createSkeleton(sbSkName);

	//Associate the skeleton to the character.
	character->setSkeleton(skeleton);

	//Set manually controlled mode on the Ogre skeleton.
	// TODO : verify that the manual control doesn't prevent the character from using classical animations.
	setManualControl(entity);

	//Add behaviors and controllers.
	character->createStandardControllers();
	retargetAllBehaviors(*character);

	return character;
}

void SmartBodyManager::setManualControl(const Ogre::Entity& entity, bool mode)
{
	Ogre::SkeletonInstance *skeleton = entity.getSkeleton();
	Ogre::Skeleton::BoneIterator it = skeleton->getBoneIterator(); 

	while (it.hasMoreElements()) 
	{ 
		it.getNext()->setManuallyControlled(mode);
	}
}

//public.
SmartBody::SBScene* SmartBodyManager::getScene(void) const
{
	return mScene;
}

//public.
SmartBody::SBAssetManager* SmartBodyManager::getAssetManager(void) const
{
	return mAssetManager;
}
	
}