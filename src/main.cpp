#include <Geode/Geode.hpp>
#include <Geode/utils/base64.hpp>
#include <string>

using namespace geode::prelude;

std::string warnings[] = {"epilepsy", "epelipsy", "epelepsy", "flashing"};

std::string descContainsWarning(GJGameLevel *level)
{
	auto descRes = base64::decodeString(level->m_levelDesc, geode::utils::base64::Base64Variant::Normal);
	std::string descLower;
	if (descRes.ok())
	{
		descLower = descRes.unwrap();
		std::transform(descLower.begin(), descLower.end(), descLower.begin(), ::tolower);
	}
	else
	{
		return "";
	}
	std::transform(descLower.begin(), descLower.end(), descLower.begin(), ::tolower);
	for (int i = 0; i < warnings->length(); i++)
	{
		if (descLower.find(warnings[i]) != std::string::npos)
		{
			return warnings[i];
		}
	}
	return "";
}

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(MyLevelInfoLayer, LevelInfoLayer)
{
	bool init(GJGameLevel *level, bool challenge)
	{
		if (!LevelInfoLayer::init(level, challenge))
		{
			return false;
		}

		std::string warning = descContainsWarning(level);

		if (warning == "")
		{
			log::info("Description does not contain epilepsy warning");
			return true;
		}
		log::info("Description contains epilepsy warning: {}", warning);

		auto epilepsyLabel = CCSprite::create("epilepsyWarning-01.png"_spr);

		auto title = this->getChildByID("title-label");
		this->addChild(epilepsyLabel);

		auto titleFirst = title->getChildByIndex(0);

		epilepsyLabel->setID("epilepsy-label"_spr);

		auto titleFirstPos = this->convertToNodeSpace(titleFirst->convertToWorldSpace(CCPointZero));
		// set pos to be directly to left of first letter in title
		epilepsyLabel->setPosition({titleFirstPos.x - (epilepsyLabel->getContentSize().width / 2) - 5, titleFirstPos.y});

		this->updateLayout();

		return true;
	}

	void onEpilepsyLabel(CCObject *)
	{
		FLAlertLayer::create("Geode", "Hello from my custom mod!", "OK")->show();
	}
};