#ifndef GD_STATE_PLUGIN_FSM
#define GD_STATE_PLUGIN_FSM

#include <assert.h>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>

class FSM;

class State {
	friend class FSM;

public:
	enum EnumStateReturn {
		NONE = 0,
		CONTINUE,
		NEW_STATE,
	};
	struct StateReturn {
		EnumStateReturn ret_enum = EnumStateReturn::NONE;
		State* new_state = nullptr;
		void* additional_data;
	};

	virtual StateReturn enter_state() {
		printf("Entering state %s \n", get_class_name());
		return StateReturn();
	}
	virtual StateReturn process(real_t delta) = 0;
	virtual StateReturn physics_process(real_t delta) = 0;
	virtual void exit_state() { printf("Leaving state %s \n", get_class_name()); }

	// here or otherplace?
	virtual StateReturn handle_input(const godot::Ref<godot::InputEvent>& p_event) = 0;

	virtual const char* get_class_name() = 0;

	// Macro to be implemented on each class inheriting this
#define CLASS_NAME(CLASS)                           \
	template <typename T>                           \
	const char* _get_class_name() {                 \
		return typeid(T).name();                    \
	}                                               \
	virtual const char* get_class_name() override { \
		return _get_class_name<CLASS>();            \
	}
};

class FSM {
protected:
	State* m_current_state = nullptr;

public:
	virtual void _enter_tree();
	virtual void _exit_tree();
	virtual void _process(real_t delta);
	virtual void _physics_process(real_t delta);
	virtual void _unhandled_input(const godot::Ref<godot::InputEvent>& p_event);

protected:
	void process_state(State::StateReturn state_return);
};

// statuses: Free, HoldingEnemy
// states: OnGround, Falling, HoldingEnemy_OnGround, HoldingEnemy_Falling
class PlayerFSM : public FSM {
};

// Data som en playerstate alltid er interessert i
//  Forer inn en ny struct som dette i hver _process
struct PlayerStateProcessContext {
	bool is_on_ground;
	Vector3 position;
	Vector3 velocity;
};
struct PlayerStateInputContext {
	Vector2 input;
	std::unordered_map<int, int> input_actions;
};

// Hver state sender requests til animation spilleren
godot::AnimationTree* m_animtree = nullptr;
// defer anim forandringer til slutt? FSM poller m_current_State etter animation oppdateringer.

#endif // GD_STATE_PLUGIN_FSM