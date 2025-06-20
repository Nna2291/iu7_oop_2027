#pragma once
#include "BaseLoadCommandDecorator.h"

class SqliteLoadCommandDecorator : public BaseLoadCommandDecorator {
    public:
        virtual ~SqliteLoadCommandDecorator() = default;
        SqliteLoadCommandDecorator() = delete;
        SqliteLoadCommandDecorator(BaseLoadCommand &command, const char *filename);
        virtual void Execute() override;
        virtual std::string GetDescription() const override;
    protected:
        const char *_filename;
};