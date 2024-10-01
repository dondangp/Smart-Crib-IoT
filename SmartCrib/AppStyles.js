import { StyleSheet } from "react-native";

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#e3f2fd",
  },
  header: {
    backgroundColor: "#0288d1",
    paddingVertical: 20,
    alignItems: "center",
    justifyContent: "center",
    flexDirection: "row",
  },
  logo: {
    width: 40,
    height: 40,
    marginRight: 10,
  },
  headerText: {
    color: "#fff",
    fontSize: 28,
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
    color: "#0288d1",
  },
  input: {
    borderColor: "#0288d1",
    borderWidth: 1,
    borderRadius: 10,
    padding: 15,
    marginVertical: 20,
    width: "80%",
    textAlign: "center",
    backgroundColor: "#ffffff",
    shadowColor: "#000",
    shadowOffset: { width: 0, height: 1 },
    shadowOpacity: 0.2,
    shadowRadius: 1.41,
    elevation: 2,
  },
  deviceButton: {
    backgroundColor: "#0288d1",
    padding: 15,
    borderRadius: 30,
    marginVertical: 10,
    width: "80%",
    alignItems: "center",
    shadowColor: "#000",
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.3,
    shadowRadius: 2.62,
    elevation: 4,
  },
  deviceButtonText: {
    color: "#fff",
    fontSize: 18,
    fontWeight: "bold",
  },
  resultText: {
    fontSize: 18,
    marginTop: 20,
    color: "#000",
  },
});

export default styles;
