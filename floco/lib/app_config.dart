import 'package:flutter/material.dart';

class Device {
  Device({required this.label, 
         required this.id,
         required this.service,
         required this.characteristic});
  
  String? label;
  String? id;
  String? service;
  String? characteristic; }
  
final List<Device> devices = [
  Device(label: 'pick picow label', id: '28:CD:C1:08:28:9F', service: 'FF10', characteristic: 'FF11'),
 //  Device(label: 'A3 dcc tester', id: '28:CD:C1:01:54:E7', service: 'FF10', characteristic: 'FF11'),
  Device(label: 'A1',              id: '28:CD:C1:08:28:9F', service: 'FF10', characteristic: 'FF11'),
  Device(label: 'A3 dcc tester', id: '28:CD:C1:01:54:E7', service: 'FF10', characteristic: 'FF11'),
]; 
  
TextStyle purple26 = TextStyle(color: Colors.purple, fontSize: 26.0, fontWeight: FontWeight.bold);
TextStyle purple20 = TextStyle(color: Colors.purple, fontSize: 20.0);
TextStyle purple18 = TextStyle(color: Colors.purple, fontSize: 18.0);
