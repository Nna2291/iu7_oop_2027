#include "BaseObjectCommand.h"

class CompositeObjectCommand : public BaseObjectCommand {
    public:
        CompositeObjectCommand() = delete;
        virtual ~CompositeObjectCommand() = default;
        CompositeObjectCommand(std::vector<size_t> ids);

        virtual void Execute() override;
        virtual std::string GetDescription() const override;


    private:
        std::vector<size_t> _ids;
};