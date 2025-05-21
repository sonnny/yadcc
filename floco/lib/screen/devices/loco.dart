
import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:numeric_selector/numeric_selector.dart';
import 'package:prompt_dialog/prompt_dialog.dart';
import '../../app_config.dart';

class Loco extends StatefulWidget {
  final BluetoothDevice device;
  Loco({super.key, required this.device});
  @override State<Loco> createState() => LocoState();}

class LocoState extends State<Loco>{

String? address = "3";
String? speed = "0";
bool directionIsForward = false;
var txChar;
  
void printServices() async {
List<BluetoothService> services = await widget.device.discoverServices();
services.forEach((service){});}
 
@override void initState(){
super.initState();
printServices(); // don't comment this out, need to discover services to function properly
txChar = BluetoothCharacteristic(
  remoteId: widget.device.remoteId,
  serviceUuid: Guid('ff10'),
  characteristicUuid: Guid('ff11'));}
 
void bleSend() async {
  String? data = "";
  if (directionIsForward) data = "sp " + address! + " " + speed! + " 1";
  else data = "sp " + address! + " " + speed! + " 0";  
  try{
  List<int>? toSend = utf8.encode(data);
  await txChar.write(toSend, withoutResponse: false);}
  catch (e) {}}
  
void function_on() async {
  String? data = "";
  String? functionOn = "159";
  data = "fn " + address! + " " + functionOn + " 1";
  try { List<int>? toSend = utf8.encode(data);
    await txChar.write(toSend, withoutResponse: false);}
  catch (e) {}}
  
void function_off() async {
  String? data = "";
  String? functionOn = "128";
  data = "fn " + address! + " " + functionOn + " 1";
  try { List<int>? toSend = utf8.encode(data);
    await txChar.write(toSend, withoutResponse: false);}
  catch (e) {}}  

void send_data() async {
  BluetoothCharacteristic c = BluetoothCharacteristic(
  remoteId: widget.device.remoteId,
  serviceUuid: Guid('ff10'),
  characteristicUuid: Guid('ff11'));
  try{ await c.write([0x41,0x42,0x31], withoutResponse: false);}
  catch (e){print(e);}}  
  
@override Widget build(BuildContext context) {
return Scaffold(appBar: AppBar(title: Text('Loco')),
body: Column(spacing: 10, children:[
  
Row(spacing: 20, children:[
    SizedBox(height:100.0),
    ElevatedButton(child: const Text('Enter loco address'),
    onPressed:() async { address = await prompt(context);
      bleSend();
      setState(() {});}),
    Text('loco address: ', style: purple20),
    Text('$address', style: purple26)
]),
  
// function buttons
Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children:[
  Column(spacing:10, children:[
  
    InkWell(onTap:function_on,
      child: Container(margin: const EdgeInsets.all(10.0), color: Colors.red[200],
        width: 60.0, height: 60.0, child: Center(child: Text('F1')))),
        
    InkWell(onTap:function_off,
      child: Container(margin: const EdgeInsets.all(10.0), color: Colors.green[200],
        width: 60.0, height: 60.0, child: Center(child: Text('F2')))),
        
    InkWell(onTap:send_data,
      child: Container(margin: const EdgeInsets.all(10.0), color: Colors.purple[200],
        width: 60.0, height: 60.0, child: Center(child: Text('F3')))),
        
    InkWell(onTap:send_data,
      child: Container(margin: const EdgeInsets.all(10.0), color: Colors.blue[200],
        width: 60.0, height: 60.0, child: Center(child: Text('F4'))))]),      

// loco speed  
    VerticalNumericSelector(
      height:400,
      minValue: 0,
      maxValue: 100,
      step: 1,
      initialValue: 0,
      onValueChanged:(value){
        speed = value.toString();
        bleSend();},
      borderRadius: BorderRadius.circular(10),
      showArrows: false,
      enableVibration: false,
      viewPort: 0.1,
      label: 'speed')]),
  
// direction, stop buttons
Row(mainAxisAlignment: MainAxisAlignment.spaceEvenly, children:[

  ElevatedButton(child: Text('direction'), onPressed:(){directionIsForward = !directionIsForward;
    bleSend();
    setState((){});},
    style:ElevatedButton.styleFrom(backgroundColor: Colors.teal[200])),

//Text('${directionIsForward}'),
    
  ElevatedButton(child: Text('stop'), onPressed:(){},
    style:ElevatedButton.styleFrom(backgroundColor: Colors.red[200]))])
]));}}    
