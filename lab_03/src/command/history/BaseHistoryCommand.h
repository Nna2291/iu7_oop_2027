#pragma once
#include "BaseCommand.h"
#include "CareTaker.h"
#include <memory>

class BaseHistoryCommand : public BaseCommand {
public:
    BaseHistoryCommand() = default;
    virtual ~BaseHistoryCommand() = default;

protected:
    static CareTaker& GetCareTaker();
};

class SaveStateCommand : public BaseHistoryCommand {
public:
    SaveStateCommand(size_t objectId);
    virtual void Execute() override;
    virtual std::string GetDescription() const override;

private:
    size_t _objectId;
};

class RestoreStateCommand : public BaseHistoryCommand {
public:
    RestoreStateCommand(size_t objectId);
    virtual void Execute() override;
    virtual std::string GetDescription() const override;

private:
    size_t _objectId;
};

class ClearHistoryCommand : public BaseHistoryCommand {
public:
    ClearHistoryCommand(size_t objectId);
    virtual void Execute() override;
    virtual std::string GetDescription() const override;

private:
    size_t _objectId;
}; 