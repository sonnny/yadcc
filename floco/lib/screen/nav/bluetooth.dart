import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import '../../app_config.dart';
import '../devices/loco.dart';

class Bluetooth extends StatefulWidget {
Bluetooth({Key? key}) : super(key: key);
@override State<Bluetooth> createState() => BluetoothState();}

class BluetoothState extends State<Bluetooth>{
late Device selectedDevice;
late BluetoothDevice bleDevice;
String? status = 'Scan';

@override void initState(){ selectedDevice = devices[0];}

void scanAndConnect() async {
  bleDevice = BluetoothDevice(remoteId: DeviceIdentifier(selectedDevice.id!));
  print(selectedDevice.id);
  await FlutterBluePlus.startScan(timeout: Duration(seconds: 10));
  await bleDevice?.connect();
  FlutterBluePlus.stopScan();
  Navigator.push(context,
    MaterialPageRoute(
      builder: (context) => Loco(device: bleDevice)));}

@override Widget build(BuildContext context) {

return Column(spacing:20, children:[SizedBox(height:10), SizedBox(height:20),

        DropdownButton(value: selectedDevice,
          hint: Text('pick picow label'),
          dropdownColor: Colors.blue[100],
          focusColor: Colors.green[100],
          icon: Icon(Icons.keyboard_arrow_down),
          items: devices.map((Device device){
            return DropdownMenuItem(value: device, child: Text(device.label!));}).toList(),
          onChanged: (Device? newDevice){
            setState((){ selectedDevice = newDevice!;});}),
            
Padding(padding:EdgeInsets.all(20), child:Card(color: Colors.red[100], 
child: ListTile(title: Text('connect to dcc'), trailing: Icon(Icons.chevron_right),
onTap: scanAndConnect))),

Padding(padding:EdgeInsets.all(20), child:Card(color: Colors.green[100], 
child: ListTile(title: Text('connect to track vehicle'), trailing: Icon(Icons.chevron_right),
onTap:(){}))),

Padding(padding:EdgeInsets.all(20), child:Card(color: Colors.purple[100], 
child: ListTile(title: Text('connect to wheel vehicle'), trailing: Icon(Icons.chevron_right),
onTap:(){}))),

]);}}
