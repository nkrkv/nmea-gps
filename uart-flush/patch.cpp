
struct State {
};

{{ GENERATED_CODE }}

// The function should be deduplicated across uart-* when XOD supports it
HardwareSerial* getSerial(int8_t no) {
#if defined(HAVE_HWSERIAL0)
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

/*
   The serial can be HardwareSerial or CDC Serial_.
   They have no common base class, thus we introduce the template
*/
template<typename TSerial>
void flush(Context ctx, TSerial* serial) {
    if (!serial)
        return;

    serial->flush();
    emitValue<output_DONE>(ctx, true);
}

void evaluate(Context ctx) {
    if (!isInputDirty<input_EXEC>(ctx))
        return;

    auto uartNo = (uint8_t)(getValue<input_UART>(ctx));

#if defined(USBCON)
    if (uartNo == 0) {
        flush(ctx, &Serial);
        return;
    }
#endif

    flush(ctx, getSerial(uartNo));
}
