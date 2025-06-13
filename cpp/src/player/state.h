#pragma once

#include <memory>
#include <optional>

#include <input/typedef.h>
#include <godot_cpp/classes/node3d.hpp>

// struct Context {
// 	godot::Node3D& owner;
// };

// enum class EState {
// 	IDLE,
// 	WALKING,
// };

// struct PlayerStateBase {
// 	virtual ~PlayerStateBase() = default;
// 	virtual auto enter(Context&) -> std::optional<EState> = 0;
// 	virtual auto exit(Context&) -> void = 0;
// 	virtual auto process(Context&, double) -> std::optional<EState> = 0;
// 	virtual auto physics_process(Context&, double) -> std::optional<EState> = 0;
// 	virtual auto handle_input(Context&, const InputState&) -> std::optional<EState> = 0;
// };
// // IdleState definition (full definition now accessible)
// class IdleState : public PlayerStateBase {
// public:
// 	virtual auto enter(Context&) -> std::optional<EState> override {return EState::WALKING;}
// 	virtual auto exit(Context&) -> void override {return;}
// 	virtual auto process(Context&, double) -> std::optional<EState> override {return {};}
// 	virtual auto physics_process(Context&, double) -> std::optional<EState> override {return {};}
// 	virtual auto handle_input(Context&, const InputState&) -> std::optional<EState> override {return {};}
// };

// // WalkingState definition (full definition now accessible)
// class WalkingState : public PlayerStateBase {
// public:
//     virtual auto enter(Context&) -> std::optional<EState> override {return {};}
// 	virtual auto exit(Context&) -> void override {return;}
// 	virtual auto process(Context&, double) -> std::optional<EState> override {return {};}
// 	virtual auto physics_process(Context&, double) -> std::optional<EState> override {return {};}
// 	virtual auto handle_input(Context&, const InputState&) -> std::optional<EState> override {return {};}
// };

// class Fsm {
// 	Context m_context;
// 	std::unique_ptr<PlayerStateBase> m_current_state;

// public:
// 	Fsm(Context t_context, EState t_start_state)
// 		: m_context(t_context)
// 		, m_current_state(allocate_new_state(t_start_state)) {
// 		m_current_state->enter(m_context);
// 	}

// 	auto process(double delta) -> void {
// 		if (m_current_state) {
// 			process_state_return(m_current_state->process(m_context, delta));
// 		}
// 	}
// 	auto physics_process(double delta) -> void {
// 		if (m_current_state) {
// 			process_state_return(m_current_state->physics_process(m_context, delta));
// 		}
// 	}
// 	auto handle_input(const InputState& input) -> void {
// 		if (m_current_state) {
// 			process_state_return(m_current_state->handle_input(m_context, input));
// 		}
// 	}

// private:
// 	auto process_state_return(std::optional<EState> new_state_opt) -> void {
// 		if (!new_state_opt) {
// 			return;
// 		}

// 		m_current_state->exit(m_context);
// 		m_current_state = allocate_new_state(new_state_opt.value());
// 		process_state_return(m_current_state->enter(m_context));
// 	}

// 	auto allocate_new_state(EState state) -> std::unique_ptr<PlayerStateBase> {
// 		switch (state) {
// 			case EState::IDLE:
// 				return std::make_unique<IdleState>();
// 			case EState::WALKING:
// 				return std::make_unique<WalkingState>();
// 		}
// 	}
// };
