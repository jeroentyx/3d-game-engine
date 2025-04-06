using System;
using System.Collections.Generic;
using System.Text;
using EosScriptCore;

namespace EosScriptCore.Scripts
{
    public enum ProcessState
    {
        Patrol,
        Chasing,
        GettingBack,
        Terminated
    }

    public enum Command
    {
        Begin,
        End,
        Pause,
        Resume,
        Exit
    }
    public class StateMachine
    {
        class StateTransition
        {
            readonly ProcessState CurrentState;
            readonly Command Command;

            public StateTransition(ProcessState currentState, Command command)
            {
                CurrentState = currentState;
                Command = command;
            }

            // Using Prime number so that hash code doesn't return a same number
            public override int GetHashCode()
            {
                return 17 + 31 * CurrentState.GetHashCode() + 31 * Command.GetHashCode();
            }

            public override bool Equals(object obj)
            {
                StateTransition other = obj as StateTransition;
                return other != null && this.CurrentState == other.CurrentState && this.Command == other.Command;
            }

        }

        Dictionary<StateTransition, ProcessState> transitions;
        public ProcessState CurrentState { get; private set; }


        public StateMachine()
        {
            CurrentState = ProcessState.Patrol;

            // State machine trasitions logic from here
            transitions = new Dictionary<StateTransition, ProcessState>
            {
                { new StateTransition(ProcessState.Patrol, Command.Exit), ProcessState.Terminated },
                { new StateTransition(ProcessState.Patrol, Command.Begin), ProcessState.Chasing },
                { new StateTransition(ProcessState.Chasing, Command.End), ProcessState.Patrol },
                { new StateTransition(ProcessState.Chasing, Command.Pause), ProcessState.GettingBack },
                { new StateTransition(ProcessState.GettingBack, Command.End), ProcessState.Patrol },
                { new StateTransition(ProcessState.GettingBack, Command.Resume), ProcessState.Chasing }
            };
        }

        // Getting Next command function from process State
        public ProcessState GetNext(Command command)
        {
            StateTransition transition = new StateTransition(CurrentState, command);
            ProcessState nextState;
            if (!transitions.TryGetValue(transition, out nextState))
            {
                throw new Exception("Invalid transition: " + CurrentState + " -> " + command);
            }
            return nextState;
        }

        public ProcessState MoveNext(Command command)
        {
            CurrentState = GetNext(command);
            return CurrentState;
        }

    }
}
