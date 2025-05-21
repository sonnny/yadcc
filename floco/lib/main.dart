import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import './screen/nav/home.dart';
import './screen/nav/help.dart';
import './screen/nav/settings.dart';
import './screen/nav/bluetooth.dart';
import './app_config.dart';

void main(){runApp(MyApp());}

class MyApp extends StatelessWidget {
  MyApp({super.key});

  @override Widget build(BuildContext context) {
    return MaterialApp(title: 'floco',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(useMaterial3: true,
        primarySwatch: Colors.blue),
      home: MainHome(currentIndex: 0));}}
      
class MainHome extends StatefulWidget {
MainHome({Key? key,  required this.currentIndex}) : super(key: key);

  int currentIndex = 0;

static const navigation = <NavigationDestination>[

  NavigationDestination(selectedIcon: Icon(Icons.home, color: Colors.green),
    icon: Icon(Icons.home_outlined, color: Colors.white), label: 'Home'),
        
  NavigationDestination(selectedIcon: Icon(Icons.account_balance_wallet, color: Colors.green),
    icon: Icon(Icons.train, color: Colors.white), label: 'BLE'),
    
  NavigationDestination(selectedIcon: Icon(Icons.warning, color: Colors.green),
    icon: Icon(Icons.help, color: Colors.white),label: 'Help'),
    
  NavigationDestination(selectedIcon: Icon(Icons.sports_soccer, color: Colors.green),
    icon: Icon(Icons.settings, color: Colors.white), label: 'Settings')];

  @override
  State<MainHome> createState() => _MainHomeState();}

class _MainHomeState extends State<MainHome> {
  late Device selectedDevice;
  late BluetoothDevice bleDevice;
  String? status = 'Scan';

  final page = [
    Home(),
    Bluetooth(),
    Help(),
    Settings()];

@override void initState(){
  selectedDevice = devices[0];}

  @override
  Widget build(BuildContext context) { return Scaffold(

    appBar: AppBar(title: Text(MainHome.navigation[widget.currentIndex].label),
      backgroundColor: Colors.green,
      foregroundColor: Colors.white), 
 
    body: page[widget.currentIndex],
      
    bottomNavigationBar:  NavigationBarTheme(
    
      data: const NavigationBarThemeData(
        indicatorColor: Colors.white,
        labelTextStyle: MaterialStatePropertyAll(TextStyle(color: Colors.white, fontSize: 11))),
          
      child: NavigationBar(
        animationDuration: const Duration(seconds: 1),
        labelBehavior: NavigationDestinationLabelBehavior.alwaysShow,
        selectedIndex: widget.currentIndex,
        height: 60,
        elevation: 0,
        backgroundColor: Colors.blueGrey,
        onDestinationSelected: (int index) {
          setState((){widget.currentIndex = index;});},
        destinations: MainHome.navigation,
)));}}

