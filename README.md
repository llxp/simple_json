# simple_json
easy to implement/use Json string to class (De-)Serializer

# USAGE:
to use the library, just perform the following steps:
1. include the "Serializable.h" in the header files of the class you want to make serializable
2. add simple_json::Serializable to the base classes of the class
3. use the "addMember(memberName, member)" to add the members you want to include in the serialization
4. use the inherited public member functions "fromString(pointer to json formatted string)" for deserialization and "toString()" to serialize the class instance

# LIMITATIONS:
1. arrays have to be of the type JsonParser::Vector<Type> to be able to work
2. every mapped class needs to be derived from "simple_json::Serializable"

# Contact:
You can contact me in github or via my email address: ![alt text](https://github.com/llxp/simple_json/blob/master/email.png?raw=true)