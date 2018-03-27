
struct State {
    ptrdiff_t capacity;
    char* buff;
    char* cursor;
    CStringView view;
};

{{ GENERATED_CODE }}

void rewind(State* state) {
    state->cursor = state->buff;
    *state->cursor = '\0';
}

bool isOverflown(State* state) {
    return state->cursor - state->buff >= state->capacity - 1;
}

void evaluate(Context ctx) {
    if (!isInputDirty<input_CHAR>(ctx))
        return;

    State* state = getState(ctx);
    if (!state->buff) {
        // Extra 1 for the terminating null
        auto cap = (size_t)getValue<input_CAP>(ctx) + 1;
        // Suboptimal. We should not use heap allocation. To make it possible
        // XOD should support compile-time constatnts. The `CAP` should be
        // demoted to a such constant then.
        state->buff = new char[cap];
        state->capacity = cap;
        state->view = CStringView(state->buff);
    }

    if (!state->cursor)
        rewind(state);

    auto c = (char)getValue<input_CHAR>(ctx);
    if (c == '\r') {
        // Ignore CR to be indifferent to line ending style
        return;
    }

    if (c == '\n') {
        emitValue<output_OUT>(ctx, XString(&state->view));
        state->cursor = nullptr; // rewind next time
        return;
    }

    if (isOverflown(state))
        return;

    *state->cursor = c;
    ++state->cursor;
    *state->cursor = '\0';

    if (length(getValue<output_OUT>(ctx)) > 0) {
        // Hide a work-in-progress result
        emitValue<output_OUT>(ctx, XString());
    }
}
