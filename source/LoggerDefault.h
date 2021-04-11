#pragma once
#include <Ultralight\platform\Logger.h>

namespace ul = ultralight;
class LoggerDefault : public ul::Logger
{
public:
	void LogMessage(ul::LogLevel loglevel, const ul::String16& messsage) override;
};
