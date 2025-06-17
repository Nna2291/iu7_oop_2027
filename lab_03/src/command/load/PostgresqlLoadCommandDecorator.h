#pragma once
#include "BaseLoadCommandDecorator.h"

class PostgresqlLoadCommandDecorator : public BaseLoadCommandDecorator {
    public:
        virtual ~PostgresqlLoadCommandDecorator() = default;
        PostgresqlLoadCommandDecorator() = delete;
        PostgresqlLoadCommandDecorator(BaseLoadCommand &command, const char *filename);
        virtual void Execute() override;
        virtual std::string GetDescription() const override;
    protected:
        const char *_filename;
}; 