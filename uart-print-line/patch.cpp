
struct State {
};

{{ GENERATED_CODE }}

// The function should be deduplicated across uart-* when XOD supports it
Stream* getStreamByUartNumber(int8_t no) {
#if defined(USBCON) || defined(HAVE_HWSERIAL0)
    if (no == 0)
        return &Serial;
#endif
#if defined(HAVE_HWSERIAL1)
    if (no == 1)
        return &Serial1;
#endif
#if defined(HAVE_HWSERIAL2)
    if (no == 2)
        return &Serial2;
#endif
#if defined(HAVE_HWSERIAL3)
    if (no == 3)
        return &Serial3;
#endif

    return nullptr;
}

void evaluate(Context ctx) {
    if (!isInputDirty<input_EXEC>(ctx))
        return;

    auto serial = getStreamByUartNumber(getValue<input_UART>(ctx));
    if (!serial)
        return;

    auto str = getValue<input_LINE>(ctx);
    for (auto it = str.iterate(); it; ++it)
        serial->write((char)*it);
    serial->write('\r');
    serial->write('\n');

    emitValue<output_DONE>(ctx, true);
}
