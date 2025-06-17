#include "LoadManager.h"
#include "SceneManager.h"
#include "TransformManager.h"
#include "DrawManager.h"
#include "BaseCommand.h"
#include "CommandManager.h"


class SceneFacade {
    public :
        SceneFacade();
        ~SceneFacade() = default;

        void Execute(BaseCommand &command);
        void ExecuteWithHistory(std::shared_ptr<BaseCommand> command);
    
    private :
        std::shared_ptr<SceneManager> _sceneManager;
        std::shared_ptr<LoadManager> _loadManager;
        std::shared_ptr<TransformManager> _transformManager;
        std::shared_ptr<DrawManager> _drawManager;
        std::shared_ptr<CommandManager> _commandManager;
};