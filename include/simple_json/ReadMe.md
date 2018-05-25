# USAGE:
to use the library, just perform the following steps:
1. include the "Serializable.h" in the header files of the class you want to make serializable
2. add simple_json:::Serializable to the base classes of the class
3. use the "addMember(memberName, member)" to add the members you want to include in the serialization

# LIMITATIONS:
1. arrays have to be of the type JsonParser::Vector<Type> to be able to work