import 'dart:ffi';

import 'package:flat_buffers/flat_buffers.dart' as fb;

typedef ConnectFuncNative = Void Function();
typedef ConnectFuncDart = void Function();

typedef GetMessageBufferFuncNative = Pointer<Uint8> Function();
typedef GetMessageBufferFuncDart = Pointer<Uint8> Function();

typedef SendFromBufferFuncNative = Void Function(Int64 size);
typedef SendFromBufferFuncDart = void Function(int size);

class EngineConnector {
  DynamicLibrary engineConnectorLib;

  late ConnectFuncDart connect;
  late GetMessageBufferFuncDart getMessageBuffer;
  late SendFromBufferFuncDart sendFromBuffer;

  // Buffer for writing messages
  late Pointer<Uint8> messageBuffer;  

  EngineConnector(this.engineConnectorLib) {
    connect = engineConnectorLib.lookupFunction<ConnectFuncNative, ConnectFuncDart>('connect');
    getMessageBuffer = engineConnectorLib.lookupFunction<GetMessageBufferFuncNative, GetMessageBufferFuncDart>('getMessageBuffer');
    sendFromBuffer = engineConnectorLib.lookupFunction<SendFromBufferFuncNative, SendFromBufferFuncDart>('sendFromBuffer');

    messageBuffer = getMessageBuffer();
  }

  void send(fb.Builder builder) {
    final size = builder.size();

    if (size > 65536) {
      throw Exception('Flatbuffers message was too large. We should dynamically reallocate here instead of throwing. This is a bug.');
    }

    // Copy message to buffer
    for (var i = 0; i < size; i++) {
      messageBuffer.elementAt(i).value = builder.buffer.elementAt(i);
    }

    // Send the message
    sendFromBuffer(size);
  }
}
