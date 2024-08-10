#ifndef GD_STATE_PLUGIN_FSM
#define GD_STATE_PLUGIN_FSM

#include <assert.h>

#include <core/core.h>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class FSM;

// Data FSM and State receives from an owner regarding _unhandled_input
struct StateInputContext {
	Vector2 input_direction;
	InputAction input_action;
};

class State {
	friend class FSM;

public:
	enum class EnumStateReturn : uint8_t {
		NONE = 0,
		CONTINUE,
		NEW_STATE,
	};
	struct StateReturn {
		EnumStateReturn ret_enum = EnumStateReturn::NONE;
		State* new_state = nullptr;
	};

	virtual StateReturn enter_state(); 	
    virtual void exit_state();

	virtual StateReturn process(real_t delta) = 0;
	virtual StateReturn physics_process(real_t delta) = 0;
	// here or otherplace?
	// virtual StateReturn handle_input(const StateInputContext& input) = 0;
	virtual StateReturn handle_input() = 0;
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
	void process(real_t delta);
	void physics_process(real_t delta);
	// void handle_input(const StateInputContext& input);
	void handle_input();

	// template <typename T>
	// void force_set_state() {
	// 	static_assert(std::is_base_of_v<State, T>, "FSM requires class State as base");
	// 	if (m_current_state) {
	// 		delete m_current_state;
	// 		m_current_state = nullptr;
	// 	}
	// 	m_current_state = new T();
	// 	assert(m_current_state != nullptr);
	// }

protected:
	void _process_state(State::StateReturn state_return);
};

#endif // GD_STATE_PLUGIN_FSM