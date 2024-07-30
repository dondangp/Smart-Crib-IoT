import React from "react";
import { StatusBar } from "expo-status-bar";
import {
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  ScrollView,
  Image,
} from "react-native";
import logo from "./logo.png";

const Header = () => (
  <View style={styles.header}>
    <Image source={logo} style={styles.logo} />
    <Text style={styles.headerText}>SmartCrib</Text>
  </View>
);

const DeviceButton = ({ name, onPress }) => (
  <TouchableOpacity style={styles.deviceButton} onPress={onPress}>
    <Text style={styles.deviceButtonText}>{name}</Text>
  </TouchableOpacity>
);

export default function App() {
  return (
    <View style={styles.container}>
      <StatusBar style="auto" />
      <Header />
      <ScrollView contentContainerStyle={styles.contentContainer}>
        <Text style={styles.title}>Connected Devices</Text>
        <DeviceButton
          name="Living Room Light"
          onPress={() => alert("Living Room Light")}
        />
        <DeviceButton name="Garage Door" onPress={() => alert("Garage Door")} />
        <DeviceButton name="Thermostat" onPress={() => alert("Thermostat")} />
        <DeviceButton
          name="Security Camera"
          onPress={() => alert("Security Camera")}
        />
        <DeviceButton
          name="Window Blinds"
          onPress={() => alert("Window Blinds")}
        />
      </ScrollView>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#e0f7fa", // Light blue background color
  },
  header: {
    backgroundColor: "#0288d1",
    padding: 20,
    alignItems: "center",
    flexDirection: "row",
    marginTop: 20, // Added marginTop to shift the header down
  },
  logo: {
    width: 200,
    height: 200,
    marginRight: 10,
  },
  headerText: {
    padding: 70,
    color: "#fff",
    fontSize: 50,
    fontWeight: "bold",
  },
  contentContainer: {
    padding: 20,
    alignItems: "center",
  },
  title: {
    fontSize: 24,
    fontWeight: "bold",
    marginVertical: 20,
  },
  deviceButton: {
    backgroundColor: "#0288d1",
    padding: 15,
    borderRadius: 10,
    marginVertical: 10,
    width: "80%",
    alignItems: "center",
  },
  deviceButtonText: {
    color: "#fff",
    fontSize: 16,
  },
});
