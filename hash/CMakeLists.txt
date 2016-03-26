define_bde_style_component(bitwise_operations)

define_bde_style_component(hash_append)

define_bde_style_component(hash_traits)
target_link_libraries(
  hash_traits.t
  PRIVATE
  rapidcheck_boost_test
  )

define_bde_style_component(integral)
target_include_directories(
  integral
  SYSTEM
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  )

define_bde_style_component(message_block)
target_link_libraries(
  message_block.t
  PRIVATE
  rapidcheck_boost_test
  )

define_bde_style_component(sha_256)
target_link_libraries(
  sha_256
  PUBLIC
  integral
  )
target_link_libraries(
  sha_256.t
  PRIVATE
  rapidcheck_boost_test
  )

define_bde_style_component(uhash)

define_bde_style_component(keccak_256)
target_link_libraries(
  keccak_256
  PUBLIC
  range-v3
  )
