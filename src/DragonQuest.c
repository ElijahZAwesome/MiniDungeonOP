#include "pebble.h"

#include "Adventure.h"
#include "Battle.h"
#include "Character.h"
#include "DragonQuest.h"
#include "Items.h"
#include "Location.h"
#include "LocationInternal.h"
#include "Logging.h"
#include "MiniDungeon.h"
#include "Monsters.h"
#include "Story.h"
	
#if INCLUDE_DRAGON_QUEST	

enum
{
	VILLAGE1 = 0,
	VILLAGE1HOUSE,
	VILLAGE2,
	VILLAGE2HOUSE,
	ROAD1,
	CELLAR1,
	CAVE1,
	FOREST1,
	FORESTCAVE1,
};

static PathClass RoadClass =
{
	.numberOfMonsters = 0,
	.monsters = {0},
	.monsterUnlockLevel = 2,
	.encounterChance = 0,
	.numberOfBackgroundImages = 3,
	.backgroundImages = {RESOURCE_ID_IMAGE_ROAD1, RESOURCE_ID_IMAGE_ROAD2, RESOURCE_ID_IMAGE_ROAD3},	
};

static PathClass CaveClass =
{
	.numberOfMonsters = 0,
	.monsters = {0},
	.monsterUnlockLevel = 2,
	.encounterChance = 0,
	.numberOfBackgroundImages = 3,
	.backgroundImages = {RESOURCE_ID_IMAGE_CAVE1, RESOURCE_ID_IMAGE_CAVE2, RESOURCE_ID_IMAGE_CAVE3},
};

static PathClass ForestClass =
{
	.numberOfMonsters = 0,
	.monsters = {0},
	.monsterUnlockLevel = 2,
	.encounterChance = 0,
	.numberOfBackgroundImages = 3,
	.backgroundImages = {RESOURCE_ID_IMAGE_FOREST1, RESOURCE_ID_IMAGE_FOREST2, RESOURCE_ID_IMAGE_FOREST3},
};

static FixedClass VillageClass =
{
	.backgroundImage = RESOURCE_ID_IMAGE_VILLAGE_EXTERIOR,
	.allowShop = true,
};

static FixedClass HouseClass =
{
	.backgroundImage = RESOURCE_ID_IMAGE_VILLAGE_INTERIOR,
	.allowShop = false,
};

static FixedClass CellarClass =
{
	.backgroundImage = RESOURCE_ID_IMAGE_CELLAR,
	.allowShop = false,
};

static FixedClass ForestCaveClass =
{
	.backgroundImage = RESOURCE_ID_IMAGE_FOREST_CAVE,
	.allowShop = false,
};

static MonsterDef monsters[] =
{
	{
		.name = "Dragon",
		.imageId = RESOURCE_ID_IMAGE_DRAGON,
		.extraFireDefenseMultiplier = 100,
		.extraIceDefenseMultiplier = 100,
		.extraLightningDefenseMultiplier = 100,
		.powerLevel = 2,
		.healthLevel = 2,
		.defenseLevel = 2,
		.magicDefenseLevel = 2,
		.allowPhysicalAttack = true,
		.allowMagicAttack = true,
		.goldScale = 10,
		.preventRun = true,
	},
};

static Location locationList[] =
{
	{
		.name = "Village 1",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 3,
		.adjacentLocations = {VILLAGE1HOUSE, ROAD1, FOREST1},
		.fixedclass = &VillageClass,
	},
	{
		.name = "Village 1 House",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {VILLAGE1, CELLAR1},
		.fixedclass = &HouseClass,
	},
	{
		.name = "Village 2",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {VILLAGE2HOUSE, ROAD1},
		.fixedclass = &VillageClass,
	},
	{
		.name = "Village 2 House",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 1,
		.adjacentLocations = {VILLAGE2},
		.fixedclass = &HouseClass,
	},
	{
		.name = "Road",
		.type = LOCATIONTYPE_PATH,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {VILLAGE1, VILLAGE2},
		.length = 10,
		.baseLevel = 1,
		.pathclass = &RoadClass,
	},
	{
		.name = "Cellar",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {VILLAGE1HOUSE, CAVE1},
		.fixedclass = &CellarClass,
	},
	{
		.name = "Cave",
		.type = LOCATIONTYPE_PATH,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {CELLAR1, FORESTCAVE1},
		.length = 20,
		.baseLevel = 1,
		.pathclass = &CaveClass,
	},
	{
		.name = "Forest",
		.type = LOCATIONTYPE_PATH,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {FORESTCAVE1, VILLAGE1},
		.length = 15,
		.baseLevel = 1,
		.pathclass = &ForestClass,
	},
	{
		.name = "Forest Cave",
		.type = LOCATIONTYPE_FIXED,
		.numberOfAdjacentLocations = 2,
		.adjacentLocations = {FOREST1, CAVE1},
		.fixedclass = &ForestCaveClass,
	},
};

StoryState dragonQuestStoryState = {0};

void InitializeDragonQuest(void)
{
	dragonQuestStoryState.needsSaving = true;
	dragonQuestStoryState.persistedStoryState.currentLocationIndex = 0;
	dragonQuestStoryState.persistedStoryState.currentLocationDuration = 0;
	dragonQuestStoryState.persistedStoryState.currentPathDestination = 0;
	dragonQuestStoryState.persistedStoryState.mostRecentMonster = 0;
	
	AddItem(ITEM_TYPE_POTION);
	AddItem(ITEM_TYPE_POTION);
	AddItem(ITEM_TYPE_POTION);
	AddItem(ITEM_TYPE_POTION);
	AddItem(ITEM_TYPE_POTION);
}

Story dragonQuestStory = 
{
	.gameNumber = DRAGON_QUEST_INDEX,
	.gameDataVersion = 3,
	.locationList = locationList,
	.monsterList = monsters,
	.initializeStory = InitializeDragonQuest,
};

void LaunchDragonQuest(void)
{
	dragonQuestStory.numberOfLocations = sizeof(locationList)/sizeof(Location);
	dragonQuestStory.numberOfMonsters = sizeof(monsters)/sizeof(MonsterDef);
	RegisterStory(&dragonQuestStory, &dragonQuestStoryState);
	DEBUG_LOG("Initialized locationList size = %d", sizeof(locationList));
	ShowAdventureWindow();
}

#endif