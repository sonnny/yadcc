
import 'package:flutter/material.dart';
import 'dart:io';
import 'dart:convert';
import 'dart:typed_data';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:numeric_selector/numeric_selector.dart';
import 'package:prompt_dialog/prompt_dialog.dart';
import '../../app_config.dart';

class Arduboard extends StatefulWidget {
  final BluetoothDevice device;
  Arduboard({super.key, required this.device});
  @override State<Arduboard> createState() => ArduboardState();}

class ArduboardState extends State<Arduboard>{

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
  
void sendOn() async {
  String? data = "ON";
  try{ List<int>? toSend = utf8.encode(data);
    await txChar.write(toSend, withoutResponse: true);}
  catch(e) {}}
  
void sendOff() async {
  String? data = "OFF";
  try{ List<int>? toSend = utf8.encode(data);
    await txChar.write(toSend, withoutResponse: true);}
  catch(e) {}}

  
@override Widget build(BuildContext context) {
return Scaffold(appBar: AppBar(title: Text('Loco')),
body: Column(spacing: 10, children:[

ElevatedButton(child: Text('on'), onPressed: sendOn),
ElevatedButton(child: Text('off'), onPressed: sendOff)]));}}

