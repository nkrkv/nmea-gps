
struct State {
    char buf[12];
    CStringView bufView;
    char* cursor;
    bool pendingReset = true;

    State() : bufView(buf) { }
};

{{ GENERATED_CODE }}

void evaluate(Context ctx) {
    if (!isInputDirty<input_PUSH>(ctx))
        return;

    State* state = getState(ctx);
    char c = (char)getValue<input_CHAR>(ctx);

    if (state->pendingReset) {
        state->cursor = state->buf;
        *state->cursor = '\0';
        state->pendingReset = false;
        // Set TOK to empty string so that downstream nodes will
        // not see a token-in-progress
        emitValue<output_TOK>(ctx, XString());
    }

    if (c == ',' || c == '\n') {
        emitValue<output_TOK>(ctx, XString(&state->bufView));
        emitValue<output_RDY>(ctx, true);
        if (c == '\n')
            emitValue<output_LINE>(ctx, true);
        state->pendingReset = true;
    } else {
        // push the character to the end of sequence
        *state->cursor++ = c;
        *state->cursor = '\0';
    }
}
