import 'dart:ffi';
import 'dart:typed_data';

typedef ConnectFuncNative = Void Function();
typedef ConnectFuncDart = void Function();

typedef GetMessageBufferFuncNative = Pointer<Uint8> Function();
typedef GetMessageBufferFuncDart = Pointer<Uint8> Function();

typedef SendFromBufferFuncNative = Void Function(Int64 size);
typedef SendFromBufferFuncDart = void Function(int size);

class EngineConnector {
  DynamicLibrary engineConnectorLib;

  late ConnectFuncDart _connect;
  late GetMessageBufferFuncDart _getMessageBuffer;
  late SendFromBufferFuncDart _sendFromBuffer;

  // Buffer for writing messages
  late Pointer<Uint8> _messageBuffer;  

  EngineConnector(this.engineConnectorLib) {
    _connect = engineConnectorLib.lookupFunction<ConnectFuncNative, ConnectFuncDart>('connect');
    _getMessageBuffer = engineConnectorLib.lookupFunction<GetMessageBufferFuncNative, GetMessageBufferFuncDart>('getMessageBuffer');
    _sendFromBuffer = engineConnectorLib.lookupFunction<SendFromBufferFuncNative, SendFromBufferFuncDart>('sendFromBuffer');

    _connect();
    _messageBuffer = _getMessageBuffer();
  }

  void send(Uint8List request) {
    final size = request.length;

    if (size > 65536) {
      throw Exception('Flatbuffers message was too large. We should dynamically reallocate here instead of throwing. This is a bug.');
    }

    // Copy message to buffer
    for (var i = 0; i < size; i++) {
      _messageBuffer.elementAt(i).value = request.elementAt(i);
    }

    // Send the message
    _sendFromBuffer(size);
  }
}
