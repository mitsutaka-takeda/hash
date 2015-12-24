#pragma once

namespace hash {
    // Still not sure the best way to implement concepts.
    //
    // Tick or Boost.ConceptCheck may be good candidates.

    // Concept HashAlgorithm

	/*    
      A type H meets the HashAlgorithm requirements if all of the following are met:

      - H::result_type is valid and denotes a MoveConstrutible type (14.8.2 [temp.deduct]).

      - H is either default constructible, or constructible by some documented seed. This construction shall initialize H to a deterministic state such that if two instances are constructed with the same arguments, then they have equivalent state.

      - H is CopyConstructible. Updates to the state of one copy shall have no impact on any other copy.

      - H is CopyAssignable. Updates to the state of one copy shall have no impact on any other copy.

      - void operator()(void const* key, std::size_t len);
         Requires: If len > 0, key points to len contiguous bytes to be consumed by the HashAlgorithm. The conversion to result_type has not been called on this object since construction, or since *this was assigned to.
   
         Effects: Updates the state of the HashAlgorithm using the len bytes referred to by {key, len}.
   
         If len == 0 then key is not dereferenced, and there are no effects.
   
         Consider two keys {k1, len1} and {k2, len2}, with len1 > 0 and len2 > 0. If len1 != len2, the two keys are considered not equivalent. If len1 == len2 and if memcmp(k1, k2, len1) == 0, the two keys are equivalent, else they are not equivalent.
   
         If two instances of HashAlgorithm (e.g. h1 and h2) have the same state prior to an update operation, and given two equivalent keys {k1, len} and {k2, len}, then after h1(k1, len) and h2(k2, len), then h1 and h2 shall have the same updated state. If {k1, len1} and {k2, len2} are not equivalent, then after h1(k1, len1) and h2(k2, len2), h1 and h2 should have different updated state.
   
         Given a key {k, len} with len > 0, one can create multiple keys each with length li, where the first key k0 == k, and subsequent keys ki == ki-1 + li-1. Combined with a constraint that ∑ li == len, the single key {k, len} shall be equivalent to the application of all of the keys {ki, li} applied in order.
   
         The HashAlgorithm shall not access this memory range after the update operation returns.
   
      - explicit operator result_type();
         Requires: This operation has not been called on this object since construction or since *this was assigned to.
   
         Effects: Converts the state of the HashAlgorithm to a result_type. Two instances of the same type of HashAlgorithm, with the same state, shall return the same value. It is unspecified if this operation changes the state of the HashAlgorithm.
   
         Returns: The converted state.
    */

} // namespace hash
