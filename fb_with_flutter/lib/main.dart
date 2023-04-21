import 'dart:ffi';
import 'dart:io';
import 'dart:typed_data';

import 'package:flat_buffers/flat_buffers.dart' as fb;

import 'package:flatbuffers_with_flutter/engine_connector.dart';
import 'package:flatbuffers_with_flutter/messages_generated.dart';
import 'package:flutter/material.dart';

late Process process;

const path = './data/flutter_assets/assets/EngineConnector.dll';
final engineConnectorLib = DynamicLibrary.open(path);

void main() async {
  // process = await Process.start('path/to/your/executable.exe', []);

  // final builder = fb.Builder();
  // final asdf = SubtractObjectBuilder(a: 5, b: 4).finish(builder);

  // final buffer = builder.buffer;
  // buffer.toPointer();

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

int idGen = 0;

class _MyHomePageState extends State<MyHomePage> {
  late EngineConnector engineConnector;
  String selectedOperator = '+';
  int left = 0;
  int right = 0;

  fb.Builder builder = fb.Builder();

  _MyHomePageState() {
    engineConnector = EngineConnector(engineConnectorLib);
  }

  void calculate() {
    Uint8List request;

    if (selectedOperator == '+') {
      request = RequestObjectBuilder(
        id: idGen++,
        commandType: CommandTypeId.Add,
        command: AddObjectBuilder(a: left, b: right),
      ).toBytes();
    } else {
      request = RequestObjectBuilder(
        id: idGen++,
        commandType: CommandTypeId.Subtract,
        command: SubtractObjectBuilder(a: left, b: right),
      ).toBytes();
    }

    engineConnector.send(request);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            SizedBox(
              height: 50,
              child: Row(
                crossAxisAlignment: CrossAxisAlignment.center,
                mainAxisSize: MainAxisSize.min,
                children: [
                  SizedBox(
                    width: 150,
                    child: TextField(
                      onChanged: (text) {
                        final value = int.tryParse(text);
                        if (value != null) {
                          left = value;
                        }
                      },
                    ),
                  ),
                  const SizedBox(width: 10),
                  DropdownMenu(
                    initialSelection: '+',
                    onSelected: (item) {
                      selectedOperator = item!;
                    },
                    dropdownMenuEntries: const [
                      DropdownMenuEntry(value: '+', label: '+'),
                      DropdownMenuEntry(value: '-', label: '-'),
                    ],
                  ),
                  const SizedBox(width: 10),
                  SizedBox(
                    width: 150,
                    child: TextField(
                      onChanged: (text) {
                        final value = int.tryParse(text);
                        if (value != null) {
                          right = value;
                        }
                      },
                    ),
                  ),
                ],
              ),
            ),
            const SizedBox(height: 40),
            MaterialButton(
              onPressed: calculate,
              child: const Text(
                'Calculate',
                style: TextStyle(fontSize: 20),
              ),
            ),
            const SizedBox(height: 40),
            MaterialButton(
              onPressed: () {
                engineConnector = EngineConnector(engineConnectorLib);
              },
              child: const Text(
                'Reconnect',
                style: TextStyle(fontSize: 20),
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  void dispose() {
    // TODO: We should send an exit command and attempt to exit gracefully

    // process.kill();

    super.dispose();
  }
}
