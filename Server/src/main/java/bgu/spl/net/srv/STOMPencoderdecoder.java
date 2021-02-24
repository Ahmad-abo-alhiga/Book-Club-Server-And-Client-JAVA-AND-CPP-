package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class STOMPencoderdecoder<T> implements MessageEncoderDecoder<T> {
    private byte[] buffer = new byte[1024];
    private int len = 0;

    @Override
    public T decodeNextByte(byte nextByte) {
        if (nextByte == '\u0000') {
            Pushbyte(nextByte);
            return popstring();
        }
        Pushbyte(nextByte);
        return null;
    }

    private T popstring() {
        String result = new String(buffer, 0, len, StandardCharsets.UTF_8);
        len = 0;
        return (T) result;
    }

    private void Pushbyte(Byte Nextbyte) {
        if (len >= buffer.length) {
            buffer = Arrays.copyOf(buffer, len * 2);
        }
        buffer[len++] = Nextbyte;
    }

    @Override
    public byte[] encode(T message) {
        return (message + "").getBytes();
    }
}
