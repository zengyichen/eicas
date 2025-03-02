#include <iostream>
#include <graphics.h>
#include "StatusInfo.h"
#include "Clock.h"
#include "Button.h"
#include "FuelValue.h"
#include "DataGenerator.h"
#include "Logger.h"
#include "Csv.h"
#include "ErrorMessage.h"

#include "Utils.h"

int main() {

	srand(time(0));

	DataGenerator aircraft;
	ExMessage mouse;
	Logger logger;
	Csv csv;

	initgraph(1080, 800);

	Clock clockEngineSpeedL(20, 40, std::bind(&DataGenerator::getEngineSpeedLVal, &aircraft), 0, 50000, std::bind(&DataGenerator::getEngineSpeedLExceptionLevel, &aircraft));
	Clock clockEngineSpeedR(180, 40, std::bind(&DataGenerator::getEngineSpeedRVal, &aircraft), 0, 50000, std::bind(&DataGenerator::getEngineSpeedRExceptionLevel, &aircraft));
	Clock clockExhaustGasTempL(20, 200, std::bind(&DataGenerator::getExhaustGasTempLVal, &aircraft), -5, 1200, std::bind(&DataGenerator::getExhaustGasTempLExceptionLevel, &aircraft));
	Clock clockExhaustGasTempR(180, 200, std::bind(&DataGenerator::getExhaustGasTempRVal, &aircraft), -5, 1200, std::bind(&DataGenerator::getExhaustGasTempRExceptionLevel, &aircraft));

	StatusInfo startStatusInfo(40, 20, "START", [&]()->bool {
		return aircraft.getStatus() == STARTING;
	});
	StatusInfo stableStatusInfo(200, 20, "RUN", [&]()->bool {
		return aircraft.getStatus() == STABLE;
	});

	Button start(350, 200, "Start", &mouse, [&]() {
		if (aircraft.getStatus() == OFF) {
			aircraft.start();
		}
	});
	Button stop(480, 200, "Stop", &mouse, [&]() {
		if (aircraft.getStatus() != OFF) {
			aircraft.stop();
		}
	});
	Button speedUp(350, 280, "Speed Up", &mouse, [&]() {
		if (aircraft.getStatus() == STABLE) {
			aircraft.speedUp();
		}
	});
	Button speedDown(480, 280, "Speed Down", &mouse, [&]() {
		if (aircraft.getStatus() == STABLE) {
			aircraft.speedDown();
		}
	});
	
	// generate trigger buttons
	std::vector<Button*> triggerButtons;
	{
		int x = 40, y = 360;

		for (ExceptionType type = ExceptionType(int(ExceptionType::NoError) + 1);
			type <= ExceptionType::OverTemp4;
			type = ExceptionType(int(type) + 1), x += 150) {
			if (x > 490) {
				x = 40;
				y += 80;
			}
			triggerButtons.push_back(new Button(x, y,
				exceptionNameMap.find(type)->second, &mouse,
				[type, &aircraft]() {
				aircraft.triggerException(type);
			}));
		}
	}

	// generate error messages
	std::vector<ErrorMessage*> errorMessages;
	{
		int y = 20;
		for (ExceptionType type = ExceptionType(int(ExceptionType::NoError) + 1);
			type <= ExceptionType::OverTemp4;
			type = ExceptionType(int(type) + 1), y += 40) {
			errorMessages.push_back(new ErrorMessage(620, y,
				errorMessageMap.find(type)->second,
				exceptionLevelMap.find(type)->second,
				[&aircraft, type]()->bool {
					return aircraft.queryException(type);
				})
			);
		}
	}


	bool quit = false;
	Button quitButton(490, 700, "Quit", &mouse, [&]() {
		quit = true;
	});


	FuelValue fuelRemain(500, 40, std::bind(&DataGenerator::getFuelRemain, &aircraft), std::bind(&DataGenerator::getFuelRemainExceptionLevel, &aircraft));
	FuelValue fuelFlow(500, 120, std::bind(&DataGenerator::getFuelFlow, &aircraft), std::bind(&DataGenerator::getFuelFlowExceptionLevel, &aircraft));

	WidgetHandler::draw();

	while (!quit) {
		if (aircraft.needUpdate()) {
			aircraft.update();
			csv.addLine(aircraft);
		}
		peekmessage(&mouse);
		std::vector<ExceptionType> errors = aircraft.updateExceptions();
		aircraft.handleExceptions(errors);

		logger.log(errors);

		WidgetHandler::update();
	}

	// release
	for (auto& msg : errorMessages) {
		delete msg;
	}

	for (auto& btn : triggerButtons) {
		delete btn;
	}

	return 0;
}
